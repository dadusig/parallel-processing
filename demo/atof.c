#include <stdio.h>
#include <stdlib.h>

#define BUFFER_SIZE 100 //in terms of itirations

int main(int argc, char const *argv[])
{
	FILE		*output1, *output2;
	output1 = fopen("myspacetime.out", "w");
	output2 = fopen("myomega.out", "w");

	int n = 4; // number of neurons
	int itime = 10; // number of itirations

	double *uplus = (double *)calloc(n, sizeof(double));

	int counter = 0, reset = 0, writen = 0;
	double *buffer = (double *)calloc(n * BUFFER_SIZE, sizeof(double));

	for (int it = 0; it < itime; it++)
	{
		for (int i = 0; i < n; i++)
		{
			uplus[i] = drand48();
			buffer[counter] = uplus[i];
			counter++;
		}

		if (counter == n * BUFFER_SIZE)
		{
			//you must flush the counter
			for (int j = 0; j < n * BUFFER_SIZE; j++)
			{
				if (j%n == 0)
				{
					writen++;
					fprintf(output1, "%ld\t", writen);
				}
				fprintf(output1, "%19.15f", buffer[j]);
				if ((j+1)%n == 0)
				{
					fprintf(output1, "\n");
				}
			}

			// reset counter
			//printf("%d, %s\n", it,"reset");
			counter = 0;
			reset++;
		}




	}

	if (counter != n * BUFFER_SIZE)
	{
		for (int j = 0; j < counter; j++)
		{
			if (j%n == 0)
			{
				writen++;
				fprintf(output1, "%ld\t", writen);
			}
			fprintf(output1, "%19.15f", buffer[j]);
			if ((j+1)%n == 0)
			{
				fprintf(output1, "\n");
			}
		}
	}
	return 0;
}
