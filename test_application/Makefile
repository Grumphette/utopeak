NBCORES=4

all: testcpu testcpu_omp testcpu_mpi  	
.PHONY: all

testcpu:
	@gcc -Wall -Wextra Src/testcpu.c -o $@

testcpu_mpi:
	@mpicc -Wall -Wextra Src/testcpu_mpi.c -o $@.$(NBCORES).C

testcpu_omp:
	@gcc -Wall -Wextra -fopenmp Src/testcpu_omp.c -o $@

clean:
	rm -f testcpu testcpu_mpi.$(NBCORES).C testcpu_omp 
