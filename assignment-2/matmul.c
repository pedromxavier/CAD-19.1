#include "matmul.h"
#include <string.h>

#ifndef USAGE
#define USAGE 	"usage:\n 0 => ./matmul ([n=2] [N=max_array_size] [step=1] [v=MIN_SAMPLE] [parallel=0])\n\
 1 => ./matmul [n] ([N=max_array_size] [step=1] [v=MIN_SAMPLE] [parallel=0])\n\
 2 => ./matmul [n] [step] ([N=max_array_size] [v=MIN_SAMPLE] [parallel=0])\n\
 3 => ./matmul [n] [step] [v] ([N=max_array_size] [parallel=0])\n\
 4 => ./matmul [n] [N] [step] [v] ([parallel=0])\n\
 5 => ./matmul [n] [N] [step] [v] [parallel]\n\
\nparallel options:\n\
 0 => sequential\n\
 1 => static scheduling\n\
 2 => guided scheduling\n\
 3 => dynamic scheduling\n\n"

#endif

#ifndef BEGIN
#define BEGIN "running ./matmul [%d] [%d] [%d] [%d] [parallel=%d]\n"
#endif


void
run_matmul(const char* fname, double (*MatMul)(double **A, double *x, double *y, int n), int n, int N, int s, int V){
    double** A;
    double*  x;
    double*  y;

	FILE* fp = fopen(fname,"w");

	double* T = vetor(V); // Times

	double mu, sigma;

	for(n; n <= N; n+=s){ // iterate over matrix sizes
        A = matriz(n, n);
        x = vetor(n);
        y = vetor(n);
        random_matriz(A, n, n);
        random_vetor(x, n);
		for(int k=0; k<V; k++){ // samples for each n
			T[k] = (*MatMul)(A, x, y, n);
		}
        free_matriz(A, n); free(x); free(y);

		mu = mean(T, V); sigma = stddev(T, V);
		fprintf(fp, "%d|%.16e$%.16e|%d\n", n, mu, sigma, V);
	}
	fclose(fp);
}

int
main(int argc, const char* argv[]){
    // int seed = time(NULL);
    srand(0);

	int n, N, s, V, parallel;

	if (argc == 2){
		if (!strcmp(argv[1], "-h")){
			printf(USAGE);
			return 0;
		}
	}
	switch(argc - 1){
        case 0:
            n = 2;
            N = -1;
            s = 1;
            V = MIN_SAMPLE;
            parallel = 0;
            break;

		case 1:
			n = atoi(argv[1]);
			N = -1;
			s = 1;
			V = MIN_SAMPLE;
			parallel = 0;
			break;
		case 2:
			n = atoi(argv[1]);
			N = -1;
			s = atoi(argv[2]);
			V = MIN_SAMPLE;
			parallel = 0;
			break;
		case 3:
			n = atoi(argv[1]);
			N = -1;
			s = atoi(argv[2]);
			V = atoi(argv[3]);
			parallel = 0;
			break;
		case 4:
			n = atoi(argv[1]);
			N = atoi(argv[2]);
			s = atoi(argv[3]);
			V = atoi(argv[4]);
			parallel = 0;
			break;
        case 5:
            n = atoi(argv[1]);
			N = atoi(argv[2]);
			s = atoi(argv[3]);
			V = atoi(argv[4]);
			parallel = atoi(argv[5]);
			break;
		default:
			printf(USAGE);
			return 1;
	}

	if (V < MIN_SAMPLE){
		V = MIN_SAMPLE;
		printf("Warning: v must be %d or greater. Assuming v = %d.\n\n", MIN_SAMPLE, MIN_SAMPLE);
	}

	if (n < 2) {
		n = 2;
		printf("Warning: n must be 2 or greater. Assuming n = 2.\n\n");
	}

	if (N == -1){
		N = max_array_size();
		}
	else if (N < n) {
		N = n;
		printf("Warning: N must be n or greater. Assuming N = n.\n\n");
	}

	printf(BEGIN, n, N, s, V, parallel);

    switch(parallel){
        case 0: //sequential
            run_matmul("results_sequential.txt", MatMul_sequential, n, N, s, V);
            break;
        case 1: //static schedule
            run_matmul("results_static_schedule.txt", MatMul_static_schedule, n, N, s, V);
            break;
        case 2: //guided schedule
            run_matmul("results_guided_schedule.txt", MatMul_guided_schedule, n, N, s, V);
            break;
        case 3: //dynamic
            run_matmul("results_dynamic_schedule.txt", MatMul_dynamic_schedule, n, N, s, V);
            break;
        default:
            printf("Unknown parallel option <%d>. program ended.\n./matmul -h for options", parallel);
    }

	return 0;
	}
