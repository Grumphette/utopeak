/*
 * UtoPeak - Automatic Estimation of DVFS Potential
 * Copyright (C) 2013 Universite de Versailles
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "CPUUtils.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <assert.h>

void setGovernor(const char* newGovernor)
{
   char filePath[100];
   int nbCPUs = 0;
   int i = 0;
   FILE* pGovernorFile;
   
   assert(newGovernor);
   
   nbCPUs = getNbCPU();
   for ( i = 0 ; i < nbCPUs ; i++ )
   {
      sprintf(filePath,"/sys/devices/system/cpu/cpu%d/cpufreq/scaling_governor",i);
      pGovernorFile = fopen(filePath,"w");
      if ( pGovernorFile == NULL )
      {
         fprintf(stderr,"Fail to open governor file : '%s'",filePath);
         continue;
      }
      fwrite(newGovernor,strlen(newGovernor)+1,1,pGovernorFile);
      fclose(pGovernorFile);
   }
}

int getNbCPU()
{
   int nbCPU = sysconf(_SC_NPROCESSORS_CONF);
   if ( nbCPU == -1 )
   {
      perror("sysconf");
   }
   
   return nbCPU;
}

int initCPUControler(CPUControler* pCPUControler)
{
   int i = 0;
   int err = 0;
   int numberOfFreq = 0;
   char filePath[100];
   
   assert(pCPUControler);
   
   pCPUControler->nbCPUs = getNbCPU();
   fprintf(stdout, "[UtoPeak : INFO] Number of CPUs: %d\n",pCPUControler->nbCPUs);
 
   FILE *cmd_freq_list = popen("cat /sys/devices/system/cpu/cpu0/cpufreq/scaling_available_frequencies | tr \" \" \"\\n\" | sort -g | sed '1d'","r");
   FILE *cmd_nb_freq = popen("cat /sys/devices/system/cpu/cpu0/cpufreq/scaling_available_frequencies|sed \"s/ /\\n/g\"|wc -l","r");
   if(!cmd_freq_list || !cmd_nb_freq)
   {
      perror("popen");
      exit(0);
   }
   
   err = fscanf(cmd_nb_freq,"%d\n",&numberOfFreq);
   assert(err >= 0);

   numberOfFreq--; //due to the white space at the end
   pclose(cmd_nb_freq);

   pCPUControler->HashFreq = malloc(sizeof(pCPUControler->HashFreq)*numberOfFreq);

   for(i=0;i < numberOfFreq;i++)
   {
      err = fscanf(cmd_freq_list,"%d",&pCPUControler->HashFreq[i]);
      assert(err >= 0);
   }
   pclose(cmd_freq_list);

   pCPUControler->pCPUControlFiles = malloc(sizeof(FILE*) * pCPUControler->nbCPUs);
   if ( pCPUControler->pCPUControlFiles == NULL )
   {
      fprintf(stderr,"Fail to allocate memory for file handler (for CPU Freq control)\n");
      return -1;
   }
   
   for ( i = 0 ; i < pCPUControler->nbCPUs ; i++ )
   {
      sprintf(filePath,"/sys/devices/system/cpu/cpu%d/cpufreq/scaling_setspeed",i);
      pCPUControler->pCPUControlFiles[i] = fopen(filePath,"w");
      if ( pCPUControler->pCPUControlFiles[i] == NULL )
      {
         fprintf(stderr,"Fail to open '%s'",filePath);
      }
   }
   
   return 0;
}

void freeCPUControler(CPUControler* pCPUControler)
{
   int i = 0;
   
   assert(pCPUControler);
   
   for ( i = 0 ; i < pCPUControler->nbCPUs ; i++ )
   {
      fclose(pCPUControler->pCPUControlFiles[i]);
   }
   free(pCPUControler->pCPUControlFiles);
   free(pCPUControler->HashFreq);
}

void changeCPUFreq(CPUControler* pCPUControler, int newFreq)
{
   int i = 0;
   char command[100] = {0};
   
   assert(pCPUControler);
   
   sprintf(command,"%d",pCPUControler->HashFreq[newFreq]);
   for ( i = 0 ; i < pCPUControler->nbCPUs ; i++ )
   {
      fseek(pCPUControler->pCPUControlFiles[i],0,SEEK_SET);
      fwrite(command,strlen(command),1,pCPUControler->pCPUControlFiles[i]);
      fflush(pCPUControler->pCPUControlFiles[i]);
   }
}
