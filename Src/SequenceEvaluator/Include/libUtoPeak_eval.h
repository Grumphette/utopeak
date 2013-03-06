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

#ifndef H_LIBUTOPEAK_EVAL
#define H_LIBUTOPEAK_EVAL

#include "CPUUtils.h"
#include "ExtLibTool.h"
#include "ExtLibTool2.h"
#include "utils.h"

// CPU Management
CPUControler cpuControler;

pthread_t watcherThread;

ProbeLib2 probeLibPFM;
ProbeLib2 probeLibCycles;
ProbeLib probeLibMSR;

long int nbTotalInstructions=0;
int currentFreq = -1;

int *sequenceTable = NULL;
size_t sequenceSize=0;

long int CHUNCK_INST = 0;

libraries probelibs;

double nbTotalMSR = 0;

void load_freqTransTable();

#endif

