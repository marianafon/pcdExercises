#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

void Print_vector(
      double    local_b[]  /* in */, 
      int       local_n    /* in */, 
      int       n          /* in */, 
      char      title[]    /* in */, 
      int       my_rank    /* in */,
      MPI_Comm  comm       /* in */,
      MPI_Datatype my_type /* in  */);
void Read_vector(
      char      prompt[]     /* in  */, 
      double    local_vec[]  /* out */, 
      int       n            /* in  */,
      int       local_n      /* in  */,
      int       my_rank      /* in  */,
      MPI_Comm  comm         /* in  */,
      MPI_Datatype my_type   /* in  */);


int main(void){
   int n, local_n, comm_sz, my_rank = 0;
   double* local_vec; 
   MPI_Comm comm;

   /* Start up MPI */
   MPI_Init(NULL, NULL); 
   comm = MPI_COMM_WORLD;
   /* Get the number of processes */
   MPI_Comm_size(MPI_COMM_WORLD, &comm_sz); 
   /* Get my rank among all the processes */
   MPI_Comm_rank(MPI_COMM_WORLD, &my_rank); 
   n = 10;
   local_n = n/comm_sz; 

   MPI_Datatype my_type;
   MPI_Type_contigous(local_n, MPI_DOUBLE, &my_type);
   MPI_Type_commit(&my_type);
   Read_vector("Test", local_vec, n, local_n, my_rank, comm, my_type);

   Print_vector(local_vec, local_n, n, "Test Vector", my_rank, comm, my_type);

   MPI_Type_free(&my_type);
   MPI_Finalize();
   return 0;
}

void Read_vector(
      char      prompt[]     /* in  */, 
      double    local_vec[]  /* out */, 
      int       n            /* in  */,
      int       local_n      /* in  */,
      int       my_rank      /* in  */,
      MPI_Comm  comm         /* in  */,
      MPI_Datatype my_type   /* in  */) {
   
   double* vec = NULL;
   int i = 0;

   if (my_rank == 0) {
      vec = malloc(n*sizeof(double));
      
      printf("Enter the vector %s\n", prompt);
      for (i = 0; i < n; i++)
         scanf("%lf", &vec[i]);
      MPI_Scatter(vec, local_n, my_type,
            local_vec, local_n, my_type, 0, comm);
      free(vec);
   } else {
      MPI_Scatter(vec, local_n, my_type,
            local_vec, local_n, my_type, 0, comm);
   }
}  /* Read_vector */

void Print_vector(
      double    local_b[]  /* in */, 
      int       local_n    /* in */, 
      int       n          /* in */, 
      char      title[]    /* in */, 
      int       my_rank    /* in */,
      MPI_Comm  comm       /* in */,
      MPI_Datatype my_type /* in  */) {

   double* b = NULL;
   int i;

   if (my_rank == 0) {
      b = malloc(n*sizeof(double));
      MPI_Gather(local_b, local_n, my_type, b, local_n, my_type,
            0, comm);
      printf("%s\n", title);
      for (i = 0; i < n; i++)
         printf("%f ", b[i]);
      printf("\n");
      free(b);
   } else {
      MPI_Gather(local_b, local_n, my_type, b, local_n, my_type, 0,
         comm);
   }
}  /* Print_vector */