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
		int id = omp_get_thread_num();
		printf("Thread no. %d created!\n", id);
		#pragma omp for
		for (int i = 0; i < N; i++)
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
