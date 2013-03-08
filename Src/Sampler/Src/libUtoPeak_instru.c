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
#include <pthread.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <assert.h>
#include <string.h>
#include <signal.h>
#include <stdlib.h>

#include "libUtoPeak_instru.h"


void dumpData()
{
   int i;

   // We are using bufferSize, since capture_sampling_end is using this function
   // to dump the remaining data
   for(i=0;i<bufferSize;i++)
   {
      fprintf(outputFile,"%f,",history[i].time);
      fprintf(outputFile,"%f,",history[i].probePFMResult);	
      fprintf(outputFile,"%f,%f,",history[i].probeTimerResult,history[i].probeMSRResult);
      fprintf(outputFile,"\n");
   }

   bufferSize = 0;
}

void takeMeasure()
{
   float timeMeasure = stopProbeMeasure2(&probeLibUnhaltedCycles);
   float pfmMeasure = stopProbeMeasure2(&probeLibRetiredInst);
   float msrMeasure = stopProbeMeasure(&probeLibMSR);

   if (timeMeasure == 0 ||
       pfmMeasure == 0 ||
       msrMeasure == 0 )
   {
      fprintf(stderr,"Fail to get valid data from the probes\n");
      fprintf(stderr,"Did you think about the pfmcounters.ini ?\n");
      return;
   }

   history[bufferSize].probeTimerResult = timeMeasure;
   history[bufferSize].probePFMResult = pfmMeasure;
   history[bufferSize].probeMSRResult = msrMeasure;
   history[bufferSize].time = ellapsedTime;
   bufferSize++;

   // The buffer is full, so we empty it
   if ( bufferSize == bufferSizeMax )
   {
      dumpData();
   }
}

void *watchman(void *data)
{
   int ret;
   struct timeval timeout;

   (void)data;

   while(1)
   {
      // Wait some time
      timeout.tv_sec = 0;
      timeout.tv_usec = 300 * 1000;
      ellapsedTime += timeout.tv_sec + timeout.tv_usec/(1000 * 1000.0f);   // Can be done after, since the timeout struct is modified by the select
      ret = select(0, NULL, NULL, NULL, &timeout);
 
      if(!ret)
      {
	  takeMeasure();
      }
   }
}


void capture_sampling_init (int pid)
{
   int ret = 0;

   probelibs = loadProbLibPath();

   loadProbeLib2(probelibs.PROBE_LIB_UNHALTED_CYCLES,&probeLibUnhaltedCycles,pid);
   loadProbeLib(probelibs.PROBE_LIB_MSR,&probeLibMSR);
   loadProbeLib2(probelibs.PROBE_LIB_RETIRED_INST,&probeLibRetiredInst,pid);

#ifdef DEBUG
   fprintf(stdout, "\n[DEBUG] : in \"%s\" from <%s>\n",__func__,__FILE__);
	fprintf(stdout, "          Sampling protocol is loaded on pid(%d), starting ...\n",pid);
#endif

   
   // Open output file
   outputFile = fopen("output.csv","w");
   if(outputFile == NULL)
   {
      perror("fopen");
      exit(1);
   }
   fprintf(outputFile,"Time,Intructions(%s),Unhalted cycles(%s),Energy(%s)\n",probelibs.PROBE_LIB_RETIRED_INST,probelibs.PROBE_LIB_UNHALTED_CYCLES,probelibs.PROBE_LIB_MSR);
	
	startProbeMeasure2(&probeLibUnhaltedCycles);
   startProbeMeasure(&probeLibMSR);
   startProbeMeasure2(&probeLibRetiredInst);


   ret = pthread_create (&watcherThread, NULL,watchman, NULL);
   if(ret)
   {
      perror("pthread_create");
      exit(1);
   }
}

void capture_sampling_end ()
{
   takeMeasure();
   pthread_cancel(watcherThread);

   // Dump the remaining data
   dumpData();

   closeProbeLib2(&probeLibUnhaltedCycles);
   closeProbeLib2(&probeLibRetiredInst);
   closeProbeLib(&probeLibMSR);

   fclose(outputFile);
#ifdef DEBUG
	fprintf(stdout, "\n[DEBUG] : in \"%s\" from <%s>\n",__func__,__FILE__);
   fprintf(stdout, "          Sampling protocol ended, quiting ...\n");
	fprintf(stdout,"----------------------------------------------\n");
#endif
}


