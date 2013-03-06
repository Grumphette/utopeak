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

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>
#include <dlfcn.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include "mpi_replace.h"
#include "capture.h"

#define SAMPLING_RANK 0

int MPI_Init(int *argc, char ***argv)
{
   int rc,rank;
   rc = PMPI_Init(argc, argv);
   MPI_Comm_rank(MPI_COMM_WORLD,&rank);

   if(rank == SAMPLING_RANK)
   {
      capture_sampling_init (getpid());
   }

	//make sure all the mpi process start the reamainder of the application at the same time
	MPI_Barrier(MPI_COMM_WORLD);
   return rc;
}


int mpi_init_(int *ierr)
{
   (void)ierr;
   
   return MPI_Init(NULL, NULL);
}

int MPI_Finalize()
{
   int rc,rank;	
   MPI_Comm_rank(MPI_COMM_WORLD,&rank);
   if(rank == SAMPLING_RANK)
   {
      capture_sampling_end ();
   }
   rc = PMPI_Finalize();
	  
   return rc;
}

int mpi_finalize_(int *ierr)
{
   (void)ierr;
   return MPI_Finalize();
}

