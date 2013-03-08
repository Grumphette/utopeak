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

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <pthread.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <assert.h>

#include "libUtoPeak_eval.h"



void getChunckInstruction()
{
   char *envNumInst = NULL;
   envNumInst = getenv("UTOPEAK_NUM_INST");
   if(envNumInst == NULL)
   {
      fprintf(stderr,"[ERROR] UTOPEAK_NUM_INST environment variable was not specified \n");
      exit(1);
   }
   CHUNCK_INST = strtol(envNumInst,NULL,10);

#ifdef DEBUG
   fprintf(stdout, "\n[DEBUG] : in \"%s\" from <%s>\n",__func__,__FILE__);
	fprintf(stdout, "          UTOPEAK_NUM_INST specified to %li \n",CHUNCK_INST);
#endif
}

int readSequenceFile(const char* fileName)
{
   assert(fileName);
   
   int arraySize = 100;
   int errorCode = 0;
   char buffer[1000]; // For frequencies 1000 should be enough
   FILE* pFile = fopen(fileName,"r");
   if ( pFile == NULL )
   {
      fprintf(stderr,"[ERROR] Sequence file '%s' failed to open\n",fileName);
      return -1;
   }
   
   int lineNumber=0;
  
   
   sequenceTable = malloc(sizeof(int) * arraySize);
   if ( sequenceTable == NULL )
   {
      fprintf(stderr,"[ERROR] Fail to allocate space for frequencies sequence\n");
      fclose(pFile);
      return -3;
   }
   
   while(fgets(buffer,1000,pFile))
   {
      int freq = -1;
      if ( sscanf(buffer,"%d",&freq) != 1 )
      {
         fprintf(stderr,"[ERROR] Fail to get the frequency from line\n'%s'\n",buffer);
         errorCode = -10;
         break;
      }
      
      sequenceTable[lineNumber] = freq;
      lineNumber++;
      if ( lineNumber >= arraySize ) // We need moar space !
      {
         arraySize *= 2;
         int* pNewArray = realloc(sequenceTable,arraySize*sizeof(int));
         if ( pNewArray == NULL )
         {
            fprintf(stderr,"[ERROR] Fail to increase size of the frequencies sequence\n");
            free(sequenceTable);
            sequenceTable = NULL;
            errorCode = -20;
            arraySize/=2;
            break;
         }
         
         sequenceTable = pNewArray;
      }
   }
   // Checks why we stop
   if (!feof(pFile))
   {
      fprintf(stderr,"[ERROR] Fail to read file\n");
      errorCode = -2;
   }
   
   sequenceSize = arraySize;
   
   fclose(pFile);
   
   return errorCode;
}


void *watchman(void *data)
{
   int ret;
   struct timeval timeout;
   unsigned int actualCalculatedStep=0;
   
   (void)data;
   
   while(1)
   {
      // Wait some time
      timeout.tv_sec = 0;
      timeout.tv_usec = 2 * 1000;
      ret = select(0, NULL, NULL, NULL, &timeout);

      if(!ret)
      {
         nbTotalInstructions = stopProbeMeasure2(&probeLibPFM);

         actualCalculatedStep = nbTotalInstructions / CHUNCK_INST;
         
         int nextFreq = sequenceTable[actualCalculatedStep];

         if(currentFreq != nextFreq)
         {
            changeCPUFreq(&cpuControler,nextFreq);
         }
         currentFreq = nextFreq;
      }
   }
}

void capture_sampling_init (int pid)
{
   int ret;

   if ( readSequenceFile("sequence.utopik") != 0 )
   {
      return;
   }
   
   getChunckInstruction();
   probelibs = loadProbLibPath();

   loadProbeLib2(probelibs.PROBE_LIB_RETIRED_INST,&probeLibPFM,pid);
   loadProbeLib(probelibs.PROBE_LIB_MSR,&probeLibMSR);

   setGovernor("userspace");
   initCPUControler(&cpuControler);

#ifdef DEBUG
   fprintf(stdout, "\n[DEBUG] : in \"%s\" from <%s>\n",__func__,__FILE__);
	fprintf(stdout, "          Sampling protocol is loaded on pid(%d), starting ...\n",pid);
#endif
	
	currentFreq = sequenceTable[0];
   changeCPUFreq(&cpuControler,currentFreq);


   startProbeMeasure2(&probeLibPFM);
   startProbeMeasure(&probeLibMSR);

   ret = pthread_create (&watcherThread, NULL,watchman, NULL);
   if(ret)
   {
      perror("pthread_create");
      exit(0);
   }
}

void capture_sampling_end ()
{
   nbTotalMSR = stopProbeMeasure(&probeLibMSR);

   pthread_cancel(watcherThread);

   closeProbeLib2(&probeLibPFM);
   closeProbeLib(&probeLibMSR);
   
   freeCPUControler(&cpuControler);
   
   free(sequenceTable);

#ifdef DEBUG
	fprintf(stdout, "\n[DEBUG] : in \"%s\" from <%s>\n",__func__,__FILE__);
   fprintf(stdout, "          Sampling protocol ended, quiting ...\n");
	fprintf(stdout,"----------------------------------------------\n");
#endif
{
	FILE *energyOutput = NULL;
	energyOutput = fopen("tmp.csv","a");
	if(energyOutput == NULL)
	{
		perror("fopen");
		exit(1);
	}
   fprintf(energyOutput,"%f",nbTotalMSR);
	fclose(energyOutput);
}	
}

