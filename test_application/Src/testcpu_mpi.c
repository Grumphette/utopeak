#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include <sys/time.h>

#include <sys/types.h>
#include <unistd.h>
#include <sched.h>

#include <mpi.h>




int main(int argc, char** argv)
{
	int i = 0;

int rank = 0;

        MPI_Init(&argc,&argv);

        MPI_Comm_size(MPI_COMM_WORLD,&rank);

	for ( i = 0 ; i < 1024*1024*128 ; i++ )
	{
		asm volatile(
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
		"addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
		"addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
		"addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
		"addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
		"addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      "addl $1,%%eax;\n\t"
      :
      :
      :"%eax"
   );
	}



	MPI_Finalize();

	return 0;
}
