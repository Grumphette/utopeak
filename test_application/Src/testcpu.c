#define _GNU_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include <sys/time.h>

#include <sys/types.h>
#include <unistd.h>
#include <sched.h>


long unsigned int getMSTime()
{
   struct timeval tv;
   long unsigned int msTime = 0;
   
   gettimeofday(&tv, NULL);
   
   msTime = tv.tv_sec * 1000 + tv.tv_usec/1000;
   
   return msTime;
}



int main()
{
	int i = 0;

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

	return 0;
}
