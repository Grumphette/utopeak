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
 
#ifndef H_LIBUTOPEAK_INSTRU
#define H_LIBUTOPEAK_INSTRU

#include <stdio.h>

#include "ExtLibTool.h"
#include "ExtLibTool2.h"
#include "utils.h"


typedef struct 
{
   int funcId;
   float time;
   double probeTimerResult;
   double probeMSRResult;
   double probePFMResult;
}HistoryLine;

FILE *outputFile = NULL;

pthread_t watcherThread;

// Data buffer management
int bufferSize = 0;
const int bufferSizeMax=100;
HistoryLine history[100];

libraries probelibs;

ProbeLib2 probeLibUnhaltedCycles;
ProbeLib probeLibMSR;
ProbeLib2 probeLibRetiredInst;

float ellapsedTime = 0;

#endif
