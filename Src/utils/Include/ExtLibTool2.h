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

#ifndef H_EXTLIBTOOL2
#define H_EXTLIBTOOL2

typedef double (*evalGet2)(void *data);
typedef  void* (*evalInit2)(int pid);
typedef int (*evalClose2)(void *data);

/* tempral function pointers */
evalGet2 tmpEvaluationStart;

typedef struct ProbeLib2
{
   void* pLibHandle;
   void* pProbeHandle;
   
	evalGet2 evaluationStart;   
	evalGet2 evaluationStop;    
	evalInit2 evaluationInit;   
	evalClose2 evaluationClose;    
   double start;
   double stop;
   double elapsed;
}ProbeLib2;

int loadProbeLib2(const char* pLibFileName, ProbeLib2* pProbeLib2, int pid);
void closeProbeLib2(ProbeLib2* pProbeLib2);
void startProbeMeasure2(ProbeLib2* pProbeLib2);
double stopProbeMeasure2(ProbeLib2* pProbeLib2);

#endif
