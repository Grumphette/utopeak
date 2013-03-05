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

#ifndef UTILS_H
#define UTILS_H

typedef struct _libraries
{
	char *PROBE_LIB_UNHALTED_CYCLES;
	char *PROBE_LIB_MSR;
	char *PROBE_LIB_RETIRED_INST;
}libraries;


void pinCPU(int cpu);
void pinThread(int cpu);
void displayThreadAffinity();

void setNice(int targetNice);

libraries loadProbLibPath();

#endif
