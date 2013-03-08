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

#include "ExtLibTool.h"

#include <stdio.h>
#include <dlfcn.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>

#ifdef DEBUG
	#include <libgen.h>
#endif


int loadProbeLib(const char* pLibFileName, ProbeLib* pProbeLib)
{
   int loadingFailure = 0;
   
   assert(pProbeLib);
   
	pProbeLib->pLibName = strdup(pLibFileName);
	
   pProbeLib->pLibHandle = dlopen(pLibFileName,RTLD_LAZY);
   if ( pProbeLib->pLibHandle == NULL )
   {
      fprintf(stderr,"[ERROR] %s\n",dlerror());
      return -1;
   }
   
   pProbeLib->evaluationInit =(evalInit) dlsym(pProbeLib->pLibHandle,"evaluationInit");
   if ( pProbeLib->evaluationInit == NULL )
   {
      fprintf(stderr,"[ERROR] Fail to load 'evaluationInit'\n");
      loadingFailure++;
      // return;
   }
   
   pProbeLib->evaluationStart =(evalGet) dlsym(pProbeLib->pLibHandle,"evaluationStart");
   if ( pProbeLib->evaluationStart == NULL )
   {
      fprintf(stderr,"[ERROR] Fail to load 'evaluationStart'\n");
      loadingFailure++;
      // return;
   }
   
   pProbeLib->evaluationStop = (evalGet) dlsym(pProbeLib->pLibHandle,"evaluationStop");
   if ( pProbeLib->evaluationStop == NULL )
   {
      fprintf(stderr,"[ERROR] Fail to load 'evaluationStop'\n");
      loadingFailure++;
      // return;
   }

   pProbeLib->evaluationClose = (evalClose) dlsym(pProbeLib->pLibHandle,"evaluationClose");
   if ( pProbeLib->evaluationClose == NULL )
   {
      fprintf(stderr,"[ERROR] Fail to load 'evaluationClose'\n");
      loadingFailure++;
      // return;
   }

#ifdef DEBUG
   if ( loadingFailure == 0 )
   {
		fprintf(stdout,"\n[DEBUG] : in \"%s\" from <%s>\n",__func__,__FILE__);
      fprintf(stdout,"          '%s' successfully loaded\n",basename(pProbeLib->pLibName));
   }
#endif
  
   // Now, we just start the probe
   if ( pProbeLib->evaluationInit )
   {
      pProbeLib->pProbeHandle = pProbeLib->evaluationInit();
   }
   return loadingFailure;
}

void closeProbeLib(ProbeLib* pProbeLib)
{
   assert(pProbeLib);
   if ( pProbeLib->pLibHandle == NULL || pProbeLib->evaluationClose == NULL )
   {
      return;
   }
   
   // Stop lib
   pProbeLib->evaluationClose(pProbeLib->pProbeHandle);
   
   dlclose(pProbeLib->pLibHandle); // Should return zero
#ifdef DEBUG
		fprintf(stdout,"\n[DEBUG] : in \"%s\" from <%s>\n",__func__,__FILE__);
      fprintf(stdout,"          '%s' successfully closed\n",basename(pProbeLib->pLibName));
#endif
	free(pProbeLib->pLibName);
}

void startProbeMeasure(ProbeLib* pProbeLib)
{
   assert(pProbeLib);
   if ( pProbeLib->pLibHandle == NULL || pProbeLib->evaluationStart == NULL )
   {
      return;
   }
   
   pProbeLib->start = pProbeLib->evaluationStart(pProbeLib->pProbeHandle);
}

double stopProbeMeasure(ProbeLib* pProbeLib)
{
   assert(pProbeLib);
   if ( pProbeLib->pLibHandle == NULL || pProbeLib->evaluationStop == NULL )
   {
      return -1;
   }
   
   pProbeLib->stop = pProbeLib->evaluationStop(pProbeLib->pProbeHandle);
   
   return pProbeLib->stop - pProbeLib->start;
}

