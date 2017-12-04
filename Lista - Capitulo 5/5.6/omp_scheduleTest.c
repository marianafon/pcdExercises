
 /* Compile: gcc -g -Wall -fopenmp -o omp_scheduleTest omp_scheduleTest.c */
 
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

void Usage(char* prog_name);

int main(int argc, char* argv[]) {
   int n;                    /* Total number of iterations    */
   int thread_count;
   int i = 0;
   int j = 0;

   if (argc != 3) Usage(argv[0]);
   thread_count = strtol(argv[1], NULL, 10);
   n = strtol(argv[2], NULL, 10);
   
   int iterations[n];            /* Iteration for thread vector   */
   
   /* Thread team creation */
# pragma omp parallel num_threads(thread_count) \
   default(none) private(i) \
   shared(n, iterations)
   {
      # pragma omp for /* For parallelization */
      for (i = 0; i < n; i++)
      {
         iterations[i] = omp_get_thread_num();
      }
   }   
   
   /* Show which iteration occurred on each thread */
   printf("\n");
   for(j = 0; j < thread_count; j++){  
      printf("Thread %d: Iterations | ", j);    
      for(i = 0; i < n; i++){      
         if(j == iterations[i])
         {
            printf("%d | ", i);            
         }
      }
      printf("\n");
   }         

   return 0;
}  /* main */

/*--------------------------------------------------------------------
 * Function:    Usage
 * Purpose:     Print command line for function and terminate
 * In arg:      prog_name
 */
void Usage(char* prog_name) {

   fprintf(stderr, "usage: %s <number of threads> <number of iterations>\n", prog_name);
   exit(0);
}  /* Usage */