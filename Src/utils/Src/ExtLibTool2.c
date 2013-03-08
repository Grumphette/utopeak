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

#include "ExtLibTool2.h"

#include <stdio.h>
#include <dlfcn.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>

#ifdef DEBUG
	#include <libgen.h>
#endif

int loadProbeLib2(const char* pLibFileName, ProbeLib2* pProbeLib2, int pid)
{
   int loadingFailure = 0;
   
   assert(pProbeLib2);
 	
	pProbeLib2->pLibName = strdup(pLibFileName);
	  
	pProbeLib2->pLibHandle = dlopen(pLibFileName,RTLD_LAZY);
   if ( pProbeLib2->pLibHandle == NULL )
   {
      fprintf(stderr,"[ERROR] %s\n",dlerror());
      return -1;
   }
   
	pProbeLib2->evaluationInit =(evalInit2) dlsym(pProbeLib2->pLibHandle,"evaluationInit");
   if ( pProbeLib2->evaluationInit == NULL )
   {
      fprintf(stderr,"[ERROR] Fail to load 'evaluationInit'\n");
      loadingFailure++;
      // return;
   }
   
	pProbeLib2->evaluationStart =(evalGet2) dlsym(pProbeLib2->pLibHandle,"evaluationStart");
   if ( pProbeLib2->evaluationStart == NULL )
   {
      fprintf(stderr,"[ERROR] Fail to load 'evaluationStart'\n");
      loadingFailure++;
      // return;
   }
   
	pProbeLib2->evaluationStop = (evalGet2) dlsym(pProbeLib2->pLibHandle,"evaluationStop");
   if ( pProbeLib2->evaluationStop == NULL )
   {
      fprintf(stderr,"[ERROR] Fail to load 'evaluationStop'\n");
      loadingFailure++;
      // return;
   }

	pProbeLib2->evaluationClose = (evalClose2) dlsym(pProbeLib2->pLibHandle,"evaluationClose");
   if ( pProbeLib2->evaluationClose == NULL )
   {
      fprintf(stderr,"[ERROR] Fail to load 'evaluationClose'\n");
      loadingFailure++;
      // return;
   }
#ifdef DEBUG
   if ( loadingFailure == 0 )
   {
		fprintf(stdout,"\n[DEBUG] : in \"%s\" from <%s>\n",__func__,__FILE__);
      fprintf(stdout,"          '%s' successfully loaded\n",basename(pProbeLib2->pLibName));
   }
#endif
  
   // Now, we just start the probe
   if ( pProbeLib2->evaluationInit )
   {
      pProbeLib2->pProbeHandle = pProbeLib2->evaluationInit(pid);
   }
   return loadingFailure;
}

void closeProbeLib2(ProbeLib2* pProbeLib2)
{
   assert(pProbeLib2);
   if ( pProbeLib2->pLibHandle == NULL || pProbeLib2->evaluationClose == NULL )
   {
      return;
   }
   
   // Stop lib
   pProbeLib2->evaluationClose(pProbeLib2->pProbeHandle);
   
   dlclose(pProbeLib2->pLibHandle); // Should return zero

#ifdef DEBUG
		fprintf(stdout,"\n[DEBUG] : in \"%s\" from <%s>\n",__func__,__FILE__);
      fprintf(stdout,"          '%s' successfully closed\n",basename(pProbeLib2->pLibName));
#endif

	free(pProbeLib2->pLibName);  
}

void startProbeMeasure2(ProbeLib2* pProbeLib2)
{
   assert(pProbeLib2);
   if ( pProbeLib2->pLibHandle == NULL || pProbeLib2->evaluationStart == NULL )
   {
      return;
   }
   
   pProbeLib2->start = pProbeLib2->evaluationStart(pProbeLib2->pProbeHandle);
}

double stopProbeMeasure2(ProbeLib2* pProbeLib2)
{
   assert(pProbeLib2);
   if ( pProbeLib2->pLibHandle == NULL || pProbeLib2->evaluationStop == NULL )
   {
      return -1;
   }
   
   pProbeLib2->stop = pProbeLib2->evaluationStop(pProbeLib2->pProbeHandle);
   
   return pProbeLib2->stop - pProbeLib2->start;
}

