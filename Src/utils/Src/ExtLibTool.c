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

int loadProbeLib(const char* pLibFileName, ProbeLib* pProbeLib)
{
   int loadingFailure = 0;
   
   assert(pProbeLib);
   
   pProbeLib->pLibHandle = dlopen(pLibFileName,RTLD_LAZY);
   if ( pProbeLib->pLibHandle == NULL )
   {
      fprintf(stderr,"[sampler : ERROR] %s\n",dlerror());
      return -1;
   }
   
   pProbeLib->evaluationInit =(evalInit) dlsym(pProbeLib->pLibHandle,"evaluationInit");
   if ( pProbeLib->evaluationInit == NULL )
   {
      fprintf(stderr,"[ProbeLibLoader : ERROR] Fail to load 'evaluationInit'\n");
      loadingFailure++;
      // return;
   }
   
   pProbeLib->evaluationStart =(evalGet) dlsym(pProbeLib->pLibHandle,"evaluationStart");
   if ( pProbeLib->evaluationStart == NULL )
   {
      fprintf(stderr,"[ProbeLibLoader : ERROR] Fail to load 'evaluationStart'\n");
      loadingFailure++;
      // return;
   }
   
   pProbeLib->evaluationStop = (evalGet) dlsym(pProbeLib->pLibHandle,"evaluationStop");
   if ( pProbeLib->evaluationStop == NULL )
   {
      fprintf(stderr,"[ProbeLibLoader : ERROR] Fail to load 'evaluationStop'\n");
      loadingFailure++;
      // return;
   }

   pProbeLib->evaluationClose = (evalClose) dlsym(pProbeLib->pLibHandle,"evaluationClose");
   if ( pProbeLib->evaluationClose == NULL )
   {
      fprintf(stderr,"[ProbeLibLoader : ERROR] Fail to load 'evaluationClose'\n");
      loadingFailure++;
      // return;
   }

   if ( loadingFailure == 0 )
   {
      fprintf(stderr,"[sampler : INFO] '%s' successfully loaded\n",pLibFileName);
   }
   
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
   
   fprintf(stderr,"[sampler : INFO] ProbeLib successfully closed\n");
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

