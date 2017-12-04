/* File:       
 *    mpi_hello.c
 *
 * Purpose:    
 *    A "hello,world" program that uses MPI
 *
 * Compile:    
 *    mpicc -g -Wall -std=C99 -o mpi_hello mpi_hello.c
 * Usage:        
 *    mpiexec -n<number of processes> ./mpi_hello
 *
 * Input:      
 *    None
 * Output:     
 *    A greeting from each process
 *
 * Algorithm:  
 *    Each process sends a message to process 0, which prints 
 *    the messages it has received, as well as its own message.
 *
 * IPP:  Section 3.1 (pp. 84 and ff.)
 */
#include <stdio.h>
#include <string.h>  /* For strlen             */
#include <mpi.h>     /* For MPI functions, etc */ 

const int MAX_STRING = 100;

int main(void) {
   char       greeting[MAX_STRING];  /* String storing message */
   char       msg[MAX_STRING];  /* String storing message */
   int        comm_sz;               /* Number of processes    */
   int        my_rank;               /* My process rank        */
   double     tempo0;
   double     tempo1;
   double     tempoTotal;
   MPI_Comm comm;
   int        count = 0;

   /* Start up MPI */
   MPI_Init(NULL, NULL); 
   comm = MPI_COMM_WORLD;
   /* Get the number of processes */
   MPI_Comm_size(MPI_COMM_WORLD, &comm_sz); 

   /* Get my rank among all the processes */
   MPI_Comm_rank(MPI_COMM_WORLD, &my_rank); 

   if (my_rank == 0){
      sprintf(greeting, "PING from process %d!",  my_rank);
      
      tempo0 = MPI_Wtime();
      //count = strlen(greeting)+1;
      printf("Count: %d\n", count);
      MPI_Send(greeting, count, MPI_CHAR, 1, 0, comm);
      //count = MAX_STRING;
      MPI_Recv(msg, count, MPI_CHAR, 1, 0, comm, MPI_STATUS_IGNORE);
      
      tempo1 = MPI_Wtime();
      tempoTotal = tempo1 - tempo0;
      
      printf("%s\n", msg);
      printf("Tempo total: %lf \n", tempoTotal);
   }else{
      //count = MAX_STRING;
      MPI_Recv(msg, count, MPI_CHAR, 0, 0, comm, MPI_STATUS_IGNORE);
      printf("Count: %d\n", count);
      printf("%s\n", msg);

      sprintf(greeting, "PONG from process %d!", my_rank);      
      //count = strlen(greeting)+1;
      printf("Count: %d\n", count);
      MPI_Send(greeting, count, MPI_CHAR, 0, 0, comm);
   }
   
   /* Shut down MPI */
   MPI_Finalize(); 

   return 0;
}  /* main */
