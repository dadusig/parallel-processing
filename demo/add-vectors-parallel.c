#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>

void print_vector(int *vector, int N, char name);
void init_vector(int *vector, int N);


int main(int argc, char const *argv[])
{
  srand(time(NULL));

  if(argc != 2)
  {
    printf("Please provide problem size\n");
    return -1;
  }

  int N = atoi(argv[1]);

  int *A, *B, *C;

  A = (int*) malloc(N * sizeof(int));
  B = (int*) malloc(N * sizeof(int));
  C = (int*) malloc(N * sizeof(int));

  if (A == NULL || B == NULL || C == NULL)
  {
    printf("Couldn't allocate memory\n");
    return -1;
  }

  init_vector(A, N);
  init_vector(B, N);

  //parallel addition starts here

  #pragma omp parallel
  {
    int i, start, end, numOfThreads, numOfElements, id; //local variables

    id = omp_get_thread_num();
    numOfThreads = omp_get_num_threads();

    numOfElements = N / numOfThreads; //number of elements per thread

    start = numOfElements * id;

    if (id == numOfThreads - 1) // am I the last thread?
    {
      end = N;
    }
    else
    {
      end = start + numOfElements;
    }

    printf("Thread no. %d will add from %d to %d\n", id, start, end-1);

    for (i = start; i < end; i++)
    {
      C[i] = A[i] + B[i];
    }
  }
  //parallel section ends here

  print_vector(A, N, 'A');
  print_vector(B, N, 'B');
  print_vector(C, N, 'C');

  return 0;
}

void print_vector(int *vector, int N, char name)
{
  printf("%c =\t", name);

  for (int i = 0; i < N; i++)
  {
    printf("%d\t", vector[i]);
  }

  printf("\n");
}

void init_vector(int *vector, int N)
{
  for (int i = 0; i < N; i++)
  {
    vector[i] = rand() % 10;
  }
}
