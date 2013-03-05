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

#ifndef _RDTSC_H_
#define _RDTSC_H_

// Define rdtscll for x86_64 arch
#ifdef __x86_64__
        #define rdtscll(val) do { \
                        unsigned int __a,__d; \
                        asm volatile("rdtsc" : "=a" (__a), "=d" (__d)); \
                        (val) = ((unsigned long)__a) | (((unsigned long)__d)<<32); \
        } while(0)
#define read_time_counter(x)  rdtscll(x)
typedef unsigned long long itc_t;
#endif

// Define rdtscll for i386 arch
#ifdef __i386__
        #define rdtscll(val) { \
                asm volatile ("rdtsc" : "=A"(val)); \
                }
#define read_time_counter(x)  rdtscll(x)
typedef unsigned long long itc_t;
#endif

#ifdef __IA64__
#include <asm/types.h>
#include <asm/intrinsics.h>
#define read_time_counter(x)  x=ia64_getreg(_IA64_REG_AR_ITC)
#define rdtscll(x) (read_time_counter((x)))
typedef long itc_t;
#endif

#endif

