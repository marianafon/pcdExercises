
/* Compile: gcc -g -Wall -fopenmp -o atomicTest atomicTest.c */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>
#include <math.h>

void Usage(char* prog_name);

int main(int argc, char* argv[]) 
{
	int thread_count;
	int n;
	double begin, end;

	if (argc != 3) Usage(argv[0]);
	thread_count = strtol(argv[2], NULL, 10);
	n = strtol(argv[1], NULL, 10); //número de iteraões

   	begin = omp_get_wtime();
#	pragma omp parallel num_threads(thread_count)
	{
		int i;
		double mySum = 0.0;
		for (i = 0; i < n; i++)
			# pragma omp atomic
			mySum += sin(i);
		//printf("Sum %f \n", mySum);
	}
	end = omp_get_wtime();

	printf("Thread count %d | Time %f s \n", thread_count, end - begin);

return 0;
} /* main */

/*--------------------------------------------------------------------
 * Function:    Usage
 * Purpose:     Print command line for function and terminate
 * In arg:      prog_name
 */
void Usage(char* prog_name) {

   fprintf(stderr, "usage: %s <number of iterations> <number of threads>\n", prog_name);
   exit(0);
}  /* Usage */