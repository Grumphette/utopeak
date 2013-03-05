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

#include "utils.h"

#define _GNU_SOURCE

#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

#include <sys/types.h>
#include <unistd.h>
#include <sched.h>

#include <pthread.h>


libraries loadProbLibPath()
{
	libraries libs;
	libs.PROBE_LIB_UNHALTED_CYCLES = getenv("CYCLES_LIB");
	libs.PROBE_LIB_MSR = getenv("ENERGY_LIB");
	libs.PROBE_LIB_RETIRED_INST = getenv("INSTRUCTION_LIB");

	if(libs.PROBE_LIB_UNHALTED_CYCLES == NULL || libs.PROBE_LIB_MSR == NULL || libs.PROBE_LIB_RETIRED_INST == NULL)
	{
		fprintf(stderr,"[Utils : ERROR] environment variable(s) :");

		if(libs.PROBE_LIB_UNHALTED_CYCLES == NULL){fprintf(stderr," CYCLES_LIB");}
		if(libs.PROBE_LIB_MSR == NULL){fprintf(stderr," ENERGY_LIB");}
		if(libs.PROBE_LIB_RETIRED_INST == NULL){fprintf(stderr," INSTRUCTION_LIB");}

		fprintf(stderr, " was(were) not specified\n");
		exit(1);
	}
	return libs;
}



void pinCPU(int cpu)
{
   cpu_set_t cpuset;
   pid_t myself = getpid();
 
   CPU_ZERO(&cpuset);
   CPU_SET(cpu,&cpuset);
 
   int ret = sched_setaffinity(myself,sizeof(cpu_set_t),&cpuset);
   if(ret != 0)
   {
      perror("sched_setaffinity");
      exit(1);
   }
}

void pinThread(int cpu)
{
   cpu_set_t cpuset;
   pthread_t myself = pthread_self();
 
   CPU_ZERO(&cpuset);
   CPU_SET(cpu,&cpuset);
 
   int ret = pthread_setaffinity_np(myself,sizeof(cpu_set_t),&cpuset);
   if(ret != 0)
   {
      perror("pthread_setaffinity_np");
      exit(1);
   }
}

int getNbCores()
{
   int nbCPU = sysconf(_SC_NPROCESSORS_CONF);
   if ( nbCPU == -1 )
   {
      perror("sysconf");
   }
   
   return nbCPU;
}

void displayThreadAffinity()
{
   cpu_set_t cpuset;
   pthread_t myself = pthread_self();

   int ret = pthread_getaffinity_np(myself,sizeof(cpu_set_t),&cpuset);
   if(ret != 0)
   {
      perror("pthread_getaffinity_np");
      exit(1);
   }

   int numberCPUs = getNbCores();
   int i = 0;
   for ( i = 0 ; i < numberCPUs ; i++ )
   {
      if ( CPU_ISSET(i,&cpuset) != 0 )
      {
         fprintf(stdout,"The thread %lu has affinity set to core : %d\n",myself,i);
      }
   }
}

void setNice(int targetNice)
{
   int newNice = 0;
   do
   {
      newNice = nice(-1);
      if ( newNice == -1 && errno == EPERM )
      {
         fprintf(stderr,"Fail to set the nice to -20 because you are not root\n");
         break;
      }
   }while ( newNice != targetNice );
}
