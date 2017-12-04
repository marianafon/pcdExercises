#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

void Read_n(int* scalar, int* n, int* local_n, int my_rank, int comm_sz, MPI_Comm comm);
void Allocate_vectors(int** local_vector_a, int** local_vector_b, int local_n, MPI_Comm comm, int comm_sz, int** sendCounts, int** displacements);
void Read_vector(int vector_a[], int vector_b[], int local_n, int n, int my_rank, MPI_Comm comm, int sendCounts[], int displacements[]);
void Print_vector(int local_vector_a[], int local_n, int n, char title[], int my_rank, MPI_Comm comm);
void Parallel_mult_vector_by_scalar(int local_vector_a[], int scalar, int local_n, int n, int my_rank, MPI_Comm comm);
void Create_SendCounts_Displacements(int comm_sz, int n, int sendCounts[], int displacements[]);

int main(void) {
   int n, local_n, scalar, local_sum, result, comm_sz, my_rank;
   int* local_vector_a;
   int* local_vector_b; 
   int* sendCounts; 
   int* displacements;
   MPI_Comm comm;
   
   /* Start up MPI */
   MPI_Init(NULL, NULL); 
   comm = MPI_COMM_WORLD;
   /* Get the number of processes */
   MPI_Comm_size(MPI_COMM_WORLD, &comm_sz); 
   /* Get my rank among all the processes */
   MPI_Comm_rank(MPI_COMM_WORLD, &my_rank); 
   /* 
    * Lê n, scalar e calcula local_n considerando que comm_sz divide n igualmente
    */
   Read_n(&scalar, &n, &local_n, my_rank, comm_sz, comm);   
   /* 
    * Envia em broadcast os valores que são necessários a todos os processos
    */
   MPI_Bcast(&n, 1, MPI_INT, 0, MPI_COMM_WORLD);
   MPI_Bcast(&scalar, 1, MPI_INT, 0, MPI_COMM_WORLD);
   /* 
    * Calcula local_n em cada processo
    */
   if(my_rank < (n/comm_sz))
   {
      local_n = (n/comm_sz) + 1;
   }
   else
   {
      local_n = (n/comm_sz);
   }
   /*
    * Aloca os vetores locais
    */
   Allocate_vectors(&local_vector_a, &local_vector_b, local_n, comm, comm_sz, &sendCounts, &displacements);
   /* 
    * Preenche os vetores sendCounts e displacements
    */
   Create_SendCounts_Displacements(comm_sz, n, sendCounts, displacements);
   /* 
    * Preenche os vetores e os distribui 
    */
   Read_vector(local_vector_a, local_vector_b, local_n, n, my_rank, comm, sendCounts, displacements);
   /* 
    * Calcula a multiplicação do vetor_a local pelo scalar e o produto escalar do resultado com o vetor_b local
    */
   int i = 0;
   for(i=0;i<local_n;i++){
      local_vector_a[i] *= scalar;
      local_vector_a[i] *= local_vector_b[i];
      local_sum += local_vector_a[i];
   }
   /*
    *Adiciona todas as somas locais ao result com destino ao processo 0
    */
   MPI_Reduce(&local_sum, &result, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
   /*
    *Imprime o resultado e libera a memória utilizada
    */
   if(my_rank == 0){
      printf("Resultado: %d\n",result);
      free(local_vector_a);
      free(local_vector_b);
   }

   /* Finaliza o MPI */
   MPI_Finalize(); 

   return 0;
}  /* main */

//---------------------------Functions------------------------------

void Read_n(int* scalar, int* n, int* local_n, int my_rank, int comm_sz, MPI_Comm comm)
{
   if (my_rank == 0) {
      printf("Qual o tamanho dos vetores?\n");
      scanf("%d", n);
      printf("Defina o escalar\n");
      scanf("%d", scalar);

      *local_n = (*n/comm_sz);
   }   
}

void Allocate_vectors(int** local_vector_a, int** local_vector_b, int local_n, MPI_Comm comm, int comm_sz, int** sendCounts, int** displacements)
{
  *local_vector_a = (int*)malloc(local_n*sizeof(int));
  *local_vector_b = (int*)malloc(local_n*sizeof(int));
  *sendCounts = (int*)malloc(comm_sz*sizeof(int));
  *displacements = (int*)malloc(comm_sz*sizeof(int));
}
   
void Create_SendCounts_Displacements(int comm_sz, int n, int sendCounts[], int displacements[])
{
  int local_n = n / comm_sz; 
  int resto = (n%comm_sz);
  int my_rank = 0;

  for(my_rank = 0; my_rank < comm_sz; my_rank++)
  {
    if(my_rank < resto){
      sendCounts[my_rank] = local_n + 1;
      displacements[my_rank] = my_rank * sendCounts[my_rank];      
    }else{
      sendCounts[my_rank] = local_n;
      displacements[my_rank] = my_rank * sendCounts[my_rank] + (resto);      
    }  
  }  
}

void Read_vector(int local_vector_a[], int local_vector_b[], int local_n, int n, int my_rank, MPI_Comm comm, int sendCounts[], int displacements[])
{
   int i = 0;
   int* vector_a = NULL;
   int* vector_b = NULL;

   if(my_rank == 0){
      vector_a = malloc(n*sizeof(int));
      vector_b = malloc(n*sizeof(int));

      printf("Defina o primeiro vetor (%d posições)\n", n);
      for(i=0;i<n;i++)
         scanf("%d", &vector_a[i]);
      
      printf("Defina o segundo vetor (%d posições)\n", n);
      for(i=0;i<n;i++)
         scanf("%d", &vector_b[i]);
      
      MPI_Scatterv(vector_a, sendCounts, displacements, MPI_INT, local_vector_a, local_n, MPI_INT, 0, MPI_COMM_WORLD);
      MPI_Scatterv(vector_b, sendCounts, displacements, MPI_INT, local_vector_b, local_n, MPI_INT, 0, MPI_COMM_WORLD);

      free(vector_a);
      free(vector_b);
   }else{
      MPI_Scatterv(vector_a, sendCounts, displacements, MPI_INT, local_vector_a, local_n, MPI_INT, 0, MPI_COMM_WORLD);
      MPI_Scatterv(vector_b, sendCounts, displacements, MPI_INT, local_vector_b, local_n, MPI_INT, 0, MPI_COMM_WORLD);
   }   
}




/*
void Print_vector(int local_vector_a[], int local_n, int n, char title[], int my_rank, MPI_Comm comm)
{
   int* vector_a = NULL;
   int i;

   if (my_rank == 0) {
      vector_a = malloc(n*sizeof(int));

      MPI_Gather(local_vector_a, local_n, MPI_DOUBLE, vector_a, local_n, MPI_DOUBLE, 0, comm);

      printf("%s\n", title);
      for (i = 0; i < n; i++)
         printf("%d ", vector_a[i]);
      printf("\n");
      free(vector_a);

   } else {
      MPI_Gather(local_vector_a, local_n, MPI_DOUBLE, vector_a, local_n, MPI_DOUBLE, 0, comm);
   }
}

void Parallel_mult_vector_by_scalar(int local_vector_a[], int scalar, int local_n, int n, int my_rank, MPI_Comm comm)
{
   int i = 0;

   for(i=0;i<local_n;i++){
      local_vector_a[i] *= scalar;
      printf("%d", local_vector_a[i]);
   }

   Print_vector(local_vector_a, local_n, n, "Primeiro vetor multiplicado pelo escalar:", my_rank, comm);
}
*/