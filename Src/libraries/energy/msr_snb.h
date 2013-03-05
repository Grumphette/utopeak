/*
Copyright (C) 2013 UVSQ

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 3
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
===============================================================================

This code was heavily inspired from the Likwid project, also under GPL V3 Licence.
You may find the Likwid project here:

http://code.google.com/p/likwid/

Likwid stands for Like I knew what I am doing. This project contributes easy to use 
command line tools for Linux to support programmers in developing high performance 
multi threaded programs.

It contains the following tools:

likwid-topology: Show the thread and cache topology
likwid-perfctr: Measure hardware performance counters on Intel and AMD processors
likwid-features: Show and Toggle hardware prefetch control bits on Intel Core 2 processors
likwid-pin: Pin your threaded application without touching your code (supports pthreads, Intel OpenMP and gcc OpenMP)
likwid-bench: Benchmarking framework allowing rapid prototyping of threaded assembly kernels
likwid-mpirun: Script enabling simple and flexible pinning of MPI and MPI/threaded hybrid applications
likwid-perfscope: Frontend for likwid-perfctr timeline mode. Allows live plotting of performance metrics.
likwid-powermeter: Tool for accessing RAPL counters and query Turbo mode steps on Intel processor.
likwid-memsweeper: Tool to cleanup ccNUMA memory domains.
*/


#ifndef H_MSR_SNB
#define H_MSR_SNB

#define POWER_POLLING_INTERVAL 20 //must be below 60 seconds since this is the worst case wrap around time

void *evaluationInit (void);
int evaluationClose (void *);
double evaluationStart (void *);
double evaluationStop (void *);

#endif
