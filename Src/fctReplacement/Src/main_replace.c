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
#include <dlfcn.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "main_replace.h"
#include "capture.h"

//single global variable for the linker to hook us in... internally used only
static hijack_main_t hijack_original_main; 


//wrapper for the original function
static int hijack_main(int argc, char** argv, char** env)
{
    setenv("LD_PRELOAD","",1); 

#ifdef DEBUG
	 fprintf(stdout,"[DEBUG] : in hijack_main from <%s>\n",__FILE__);
	 fprintf(stdout,"          pining core map : %s\n",getenv("GOMP_CPU_AFFINITY"));
	 fprintf(stdout,"          watching pid : %d\n",getpid());
#endif
  
    atexit(capture_sampling_end);
    capture_sampling_init (getpid());
    
    return hijack_original_main(argc, argv, env);
}


// we redefine libc_start_main because at this point main is an arguement so we can store it in our symbol table and hook in
int __libc_start_main(hijack_main_t main, int argc, char** ubp_av,hijack_lsm_t init, hijack_lsm_t fini, hijack_lsm_t rtld_fini, void* stack_end)
{
    //reset main to our global variable so our wrapper can call it easily
    hijack_original_main = main;
    //Initialisation :

    hijack_libc_start_main_t real_start =(hijack_libc_start_main_t)dlsym(RTLD_NEXT, "__libc_start_main");

    
    //call the wrapper with the real libc_start_main
    return real_start(hijack_main, argc, ubp_av, init, fini, rtld_fini, stack_end);
}


