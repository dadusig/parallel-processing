#include <stdio.h>
#include <math.h>
#include <getopt.h>
#include <stdlib.h>
#include <sys/time.h>
#include <cblas.h>
#include <omp.h>

#define MIN_NUM_OF_NEURONS	(1L)
#define DEF_NUM_OF_NEURONS	(1000L)

#define MIN_NUM_OF_NEIGHBORS	(0L)
#define DEF_NUM_OF_NEIGHBORS	(300L)

#define DEF_DT			(1.0e-04)
#define DEF_MU			(1.0)
#define DEF_UTH			(0.98)
#define	DEF_S_MIN		(0.7)
#define DEF_S_MAX		(0.7)
#define DEF_SIM_TIME		(20L)
#define DEF_TTRANSIENT		(-1L)

#define DEF_BUFFER_SIZE 10000

#define UNUSED(expr) do { (void)(expr); } while (0)

static struct option long_options[] =
{
	{"dt",        required_argument, 0, 'a'},
	{"mu",        required_argument, 0, 'b'},
	{"uth",       required_argument, 0, 'c'},
	{"time",      required_argument, 0, 'd'},
	{"transient", required_argument, 0, 'e'},
	{"s_min",     required_argument, 0, 'f'},
	{"s_max",     required_argument, 0, 'g'},
	{"n",         required_argument, 0, 'n'},
	{"r",         required_argument, 0, 'r'},
	{"x",         required_argument, 0, 'x'},
	{0, 0, 0, 0}
};

int main(int argc, char *argv[])
{
	FILE		*output1, *output2;
	long		n, r;
	long		i, j;
	long		it;
	double		divide;
	double		dt;
	double		tstep;
	long		ntstep;
	long		sim_time;
	long		ttransient;
	long		itime;
	long		buffer_size;
	double		uth;
	double		mu;
	double		s_min;
	double		s_max;
	double		*u, *uplus, *sigma, *omega, *omega1, *temp, *sigma_vector;
	double		sum;
	double		time;
	struct timeval	global_start, global_end, IO_start, IO_end;
	double		global_usec, IO_usec = 0.0;
	int		c, option_index;
	char		*end_ptr;

	n          = DEF_NUM_OF_NEURONS;
	r          = DEF_NUM_OF_NEIGHBORS;
	dt         = DEF_DT;
	mu         = DEF_MU;
	uth        = DEF_UTH;
	s_min      = DEF_S_MIN;
	s_max      = DEF_S_MAX;
	sim_time   = DEF_SIM_TIME;
	ttransient = DEF_TTRANSIENT;
	buffer_size = DEF_BUFFER_SIZE;

	while (1) {
		c = getopt_long (argc, argv, "+n:r:", long_options, &option_index);

		if (c == -1) {
			break;
		}

		switch (c) {
			case 'a':
			dt = strtod(optarg, &end_ptr);
			if (*end_ptr != '\0') {
				printf("Option \"%s\": Invalid argument \"%s\".\n", long_options[option_index].name, optarg);
				exit(1);
			}
			if (dt <= 0.0) {
				printf("Option \"%s\": \"dt\" must be larger than zero.\n", long_options[option_index].name);
				exit(1);
			}
			break;
			case 'b':
			mu = strtod(optarg, &end_ptr);
			if (*end_ptr != '\0') {
				printf("Option \"%s\": Invalid argument \"%s\".\n", long_options[option_index].name, optarg);
				exit(1);
			}
			if (mu <= 0.0) {
				printf("Option \"%s\": \"mu\" must be larger than zero.\n", long_options[option_index].name);
				exit(1);
			}
			break;
			case 'c':
			uth = strtod(optarg, &end_ptr);
			if (*end_ptr != '\0') {
				printf("Option \"%s\": Invalid argument \"%s\".\n", long_options[option_index].name, optarg);
				exit(1);
			}
			if (uth <= 0.0) {
				printf("Option \"%s\": \"uth\" must be larger than zero.\n", long_options[option_index].name);
				exit(1);
			}
			break;
			case 'd':
			sim_time = strtol(optarg, &end_ptr, 10);
			if (*end_ptr != '\0') {
				printf("Option \"%s\": Invalid argument \"%s\".\n", long_options[option_index].name, optarg);
				exit(1);
			}
			if (sim_time < 1) {
				printf("Option \"%s\": Total simulation time must be larger than zero.\n", long_options[option_index].name);
				exit(1);
			}
			break;
			case 'e':
			ttransient = strtol(optarg, &end_ptr, 10);
			if (*end_ptr != '\0') {
				printf("Option \"%s\": Invalid argument \"%s\".\n", long_options[option_index].name, optarg);
				exit(1);
			}
			if (ttransient < 0) {
				printf("Option \"%s\": \"ttransient\" must be larger or equal than zero.\n", long_options[option_index].name);
				exit(1);
			}
			break;
			case 'f':
			s_min = strtod(optarg, &end_ptr);
			if (*end_ptr != '\0') {
				printf("Option \"%s\": Invalid argument \"%s\".\n", long_options[option_index].name, optarg);
				exit(1);
			}
			if (s_min <= 0.0) {
				printf("Option \"%s\": \"s_min\" must be larger than zero.\n", long_options[option_index].name);
				exit(1);
			}
			break;
			case 'g':
			s_max = strtod(optarg, &end_ptr);
			if (*end_ptr != '\0') {
				printf("Option \"%s\": Invalid argument \"%s\".\n", long_options[option_index].name, optarg);
				exit(1);
			}
			if (s_max <= 0.0) {
				printf("Option \"%s\": \"s_max\" must be larger than zero.\n", long_options[option_index].name);
				exit(1);
			}
			break;
			case 'n':
			n = strtol(optarg, &end_ptr, 10);
			if (*end_ptr != '\0') {
				printf("Option \"%s\": Invalid argument \"%s\".\n", long_options[option_index].name, optarg);
				exit(1);
			}
			if (n < MIN_NUM_OF_NEURONS) {
				printf("Option \"%s\": Number of neurons must be at least %ld.\n", long_options[option_index].name, MIN_NUM_OF_NEURONS);
				exit(1);
			}
			break;
			case 'r':
			r = strtol(optarg, &end_ptr, 10);
			if (*end_ptr != '\0') {
				printf("Option \"%s\": Invalid argument \"%s\".\n", long_options[option_index].name, optarg);
				exit(1);
			}
			if (r < MIN_NUM_OF_NEIGHBORS) {
				printf("Option \"%s\": Number of neighbors must be at least %ld.\n", long_options[option_index].name, MIN_NUM_OF_NEIGHBORS);
				exit(1);
			}
			break;
			case 'x':
			buffer_size = strtol(optarg, &end_ptr, 10);
			if (*end_ptr != '\0') {
				printf("something went wrong with buffer option");
				exit(1);
			}
			break;
			case '?':
			default:
			exit(1);
			break;
		}
	}

	if (optind != argc) {
		printf("Unknown option \"%s\".\n", argv[optind]);
		exit(1);
	}

	if (2 * r + 1 > n) {
		printf("Total number of neighbors and reference neuron (2 * %ld + 1 = %ld) cannot exceed number of neurons (%ld).\n", r, 2 * r + 1, n);
		exit(1);
	}

	if (s_min > s_max) {
		printf("s_min (%17.15f) must be smaller or equal than s_max (%17.15f).\n", s_min, s_max);
		exit(1);
	}

	divide = (double)(2 * r);
	tstep = 1.0 / dt;
	ntstep = (long)tstep;
	if (ttransient == DEF_TTRANSIENT) {
		ttransient = (sim_time * ntstep) / 2;
	} else {
		ttransient *= ntstep;
	}
	itime = sim_time * ntstep;

	printf("Running simulation with following parameters:\n");
	printf("  Number of neurons   : %ld\n", n);
	printf("  Numger of neighbours: %ld\n", r);
	printf("  Simulation time     : %ld seconds (%ld time steps)\n", sim_time, itime);
	printf("  Transient time      : %ld seconds (%ld time steps)\n", ttransient / ntstep, ttransient);
	printf("  dt                  : %.1e seconds \n", dt);
	printf("  mu                  : %17.15f\n", mu);
	printf("  uth                 : %17.15f\n", uth);
	printf("  s_min               : %17.15f\n", s_min);
	printf("  s_max               : %17.15f\n", s_max);
	printf("  buffer_size         : %ld (number of iterations buffered)\n", buffer_size);


	output1 = fopen("spacetime.out", "w");
	if (output1 == NULL) {
		printf("Could not open file \"spacetime.out\"");
		exit(1);
	}

	output2 = fopen("omega.out", "w");
	if (output2 == NULL) {
		printf("Could not open file \"omega.out\"");
		exit(1);
	}

	u = (double *)calloc(n, sizeof(double));
	if (u == NULL) {
		printf("Could not allocate memory for \"u\".\n");
		exit(1);
	}

	uplus = (double *)calloc(n, sizeof(double));
	if (uplus == NULL) {
		printf("Could not allocate memory for \"uplus\".\n");
		exit(1);
	}

	sigma = (double *)calloc(n * n, sizeof(double));
	if (sigma == NULL) {
		printf("Could not allocate memory for \"sigma\".\n");
		exit(1);
	}

	omega = (double *)calloc(n, sizeof(double));
	if (omega == NULL) {
		printf("Could not allocate memory for \"omega\".\n");
		exit(1);
	}

	omega1 = (double *)calloc(n, sizeof(double));
	if (omega1 == NULL) {
		printf("Could not allocate memory for \"omega1\".\n");
		exit(1);
	}

	/*
	* Initialize elements of array u[i] with random numbers.
	*/
	for (i = 0; i < n; i++ ) {
		u[i] = drand48();
		printf("%ld\t%f\n", i, u[i]);
	}

	/*
	* Read connectivity matrix sigma[n][n] from file or
	* construct connectivity matrix.
	*/
	for (i = 0; i < r; i++) {
		for (j = 0; j < i + r + 1; j++) {
			sigma[i * n + j] = s_min + (s_max - s_min) * drand48();
		}
		for (j = n - r + i; j < n; j++) {
			sigma[i * n + j] = s_min + (s_max - s_min) * drand48();
		}
	}

	for (i = r; i < n - r; i++) {
		for (j = 0; j < 2 * r + 1; j++) {
			sigma[i * n + j + i - r]  = s_min + (s_max - s_min) * drand48();
		}
	}

	for (i = n - r; i < n; i++) {
		for (j = 0; j < i - n + r + 1; j++) {
			sigma[i * n + j] = s_min + (s_max - s_min) * drand48();
		}
		for (j = i - r; j < n; j++) {
			sigma[i * n + j] = s_min + (s_max - s_min) * drand48();
		}
	}
	#if 0
	for (i = 0; i < n; i++) {
	for (j = 0; j < n; j++) {
	printf("%4.1f", sigma[i * n + j]);
}
printf("\n");
}
#endif

/*
* Temporal iteration.
*/
//CEID 1b
double var2 = dt / divide;

sigma_vector = (double *)calloc(n, sizeof(double));
for (i = 0; i < n; i++) {
	sigma_vector[i] = 0.0;
	for (j = 0; j < n; j++) {
		sigma_vector[i] += (-sigma[i*n + j]);
	}
}

double *mysum1;
mysum1 = (double *)calloc(n, sizeof(double));

long counter = 0, reset = 0, writen = 0;
double *buffer_space = (double *)calloc(n * buffer_size, sizeof(double));
double *buffer_omega = (double *)calloc(n * buffer_size, sizeof(double));

gettimeofday(&global_start, NULL);
for (it = 0; it < itime; it++) {
	/*
	* Iteration over elements.
	*/
	#pragma omp parallel private(i, sum) num_threads(8)
	{
		// int start, end, numOfThreads, numOfElements, id;
		//
		int numOfThreads = omp_get_num_threads();
		int numOfElements = n / numOfThreads;
		int id = omp_get_thread_num();

		double* sigma_new = sigma + id * numOfElements * n;
		//double* u_new = u + id * numOfElements;
		double* mysum1_new = mysum1 + id * numOfElements;

		int M;

		if (id != numOfThreads - 1) {
			M = n/numOfThreads;
		}
		else
		{
			M = n/numOfThreads + n%numOfThreads;
		}


		cblas_dgemv(CblasRowMajor, CblasNoTrans, M, n, 1, sigma_new, n, u, 1, 0, mysum1_new, 1);
		//?gemv(                       'T',      M, N, a,   A,   M,  X, 1, b,  Y,  1)


		#pragma omp for
		for (i = 0; i < n; i++) {
			uplus[i] = u[i] + dt * (mu - u[i]);
			sum = 0.0;
			/*
			* Iteration over neighbouring neurons.
			*/

			sum = mysum1[i] + u[i]*sigma_vector[i];

			uplus[i] += var2 * sum;
		}
	}

	/*
	* Update network elements and set u[i] = 0 if u[i] > uth
	*/

	//CEID 1a
	temp = uplus;
	uplus = u;
	u = temp;

	time = (double)it * dt;
	for (i = 0; i < n; i++) {
		buffer_space[counter] = u[i]; //giati uplus -> i
		if (u[i] > uth) {
			u[i] = 0.0;
			buffer_space[counter] = 0.0;
			/*
			* Calculate omega's.
			*/
			if (it >= ttransient) {
				omega1[i] += 1.0;
			}
		}
		omega[i] = 2.0 * M_PI * omega1[i] / (time - ttransient * dt);
		buffer_omega[counter] = omega[i];
		counter++;
	}

	/*
	* Print out of results.
	*/
	#if !defined(ALL_RESULTS)
	if (it % ntstep == 0)
	{

		printf("Time is %ld\n", it);

		gettimeofday(&IO_start, NULL);
		fprintf(output1, "%ld\t", it);
		fprintf(output2, "%ld\t", it);
		for (i = 0; i < n; i++) {
			fprintf(output1, "%19.15f", u[i]);
			fprintf(output2, "%19.15f", omega[i]);
		}
		fprintf(output1, "\n");
		fprintf(output2, "\n");

		gettimeofday(&IO_end, NULL);
		IO_usec += ((IO_end.tv_sec - IO_start.tv_sec) * 1000000.0 + (IO_end.tv_usec - IO_start.tv_usec));
	}
	UNUSED(reset);
	#else
	if (it % ntstep == 0)
	printf("Time is %ld\n", it);

	gettimeofday(&IO_start, NULL);
	// fprintf(output1, "%ld\t", it);
	// fprintf(output2, "%ld\t", it);
	// for (i = 0; i < n; i++) {
	// 	fprintf(output1, "%19.15f", u[i]);
	// 	fprintf(output2, "%19.15f", omega[i]);
	// }
	// fprintf(output1, "\n");
	// fprintf(output2, "\n");


	if (counter == n * buffer_size)
	{
		//you must flush the counter
		printf("%s\n", "-------------------> i/o wait");
		for (j = 0; j < n * buffer_size; j++)
		{
			if (j%n == 0)
			{
				fprintf(output1, "%ld\t", writen);
				// fprintf(output1, "%ld\t", writen);
				fprintf(output2, "%ld\t", writen);
				writen++;
			}
			//printf("%19.15f\n", buffer_space[j]);
			fprintf(output1, "%19.15f", buffer_space[j]);
			fprintf(output2, "%19.15f", buffer_omega[j]);
			if ((j+1)%n == 0)
			{
				fprintf(output1, "\n");
				fprintf(output2, "\n");
			}
		}

		// reset counter
		//printf("%d, %s\n", it,"reset");
		counter = 0;
		reset++;
	}

	gettimeofday(&IO_end, NULL);
	IO_usec += ((IO_end.tv_sec - IO_start.tv_sec) * 1000000.0 + (IO_end.tv_usec - IO_start.tv_usec));
	#endif

}





gettimeofday(&IO_start, NULL);
if (counter != n * buffer_size)
{
	for (j = 0; j < counter; j++)
	{
		if (j%n == 0)
		{
			writen++;
			fprintf(output1, "%ld\t", writen);
			fprintf(output2, "%ld\t", writen);
		}
		fprintf(output1, "%19.15f", buffer_space[j]);
		fprintf(output2, "%19.15f", buffer_omega[j]);
		if ((j+1)%n == 0)
		{
			fprintf(output1, "\n");
			fprintf(output2, "\n");
		}
	}
}
gettimeofday(&IO_end, NULL);
IO_usec += ((IO_end.tv_sec - IO_start.tv_sec) * 1000000.0 + (IO_end.tv_usec - IO_start.tv_usec));





gettimeofday(&global_end, NULL);
global_usec = ((global_end.tv_sec - global_start.tv_sec) * 1000000.0 + (global_end.tv_usec - global_start.tv_usec));

printf("Time for calculations = %13.6f sec\n", (global_usec - IO_usec) / 1000000.0);
printf("Time for I/O          = %13.6f sec\n", IO_usec / 1000000.0);
printf("Total execution time  = %13.6f sec\n", global_usec / 1000000.0);

fclose(output1);
fclose(output2);

free(u);
free(uplus);
free(buffer_space);
free(buffer_omega);

return 0;
}
