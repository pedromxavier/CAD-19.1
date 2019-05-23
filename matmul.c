#include "matmul.h"
#include <string.h>

#ifndef USAGE
#define USAGE "usage:\n [0]./matmul ([n=2] [N=max_array_size] [step=1] [v=MIN_SAMPLE]) \n\n [1]./matmul [n] ([N=max_array_size] [step=1] [v=MIN_SAMPLE]) \n\n [2]./matmul [n] [step] ([N=max_array_size] [v=MIN_SAMPLE]) \n\n [3]./matmul [n] [step] [v] ([N=max_array_size]) \n\n [4]./matmul [n] [N] [step] [v]\n\n"
#endif

#ifndef BEGIN
#define BEGIN "running ./matmul [%d] [%d] [%d] [%d]\n"
#endif

int
main(int argc, const char* argv[]){
    int seed = time(NULL);
    srand(seed);

	int n, N, s, V;

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
            break;

		case 1:
			n = atoi(argv[1]);
			N = -1;
			s = 1;
			V = MIN_SAMPLE;
			break;
		case 2:
			n = atoi(argv[1]);
			N = -1;
			s = atoi(argv[2]);
			V = MIN_SAMPLE;
			break;
		case 3:
			n = atoi(argv[1]);
			N = -1;
			s = atoi(argv[2]);
			V = atoi(argv[3]);
			break;
		case 4:
			n = atoi(argv[1]);
			N = atoi(argv[2]);
			s = atoi(argv[3]);
			V = atoi(argv[4]);
			break;
		default:
			printf(USAGE);
			return 1;
	}



	if (V < MIN_SAMPLE){
		V = MIN_SAMPLE;
		printf("Warning: v must be %d or greater. Assuming v = %d.\n\n", MIN_SAMPLE, MIN_SAMPLE);
	}

	double* T_c = vetor(V);
	double* T_f = vetor(V);

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

	printf(BEGIN, n, N, s, V);

	// initializes with maximum sizes
	double** A = matriz(N, N);
    double*  x = vetor(N);
    double*  y = vetor(N);

	FILE* fp = fopen("result.txt","w");

	double* T;

	double mu, sigma;

	int k;

	for(n; n <= N; n+=s){ // iterate over matrix sizes
		T = vetor(V);

		for(k=0; k<V; k++){ // samples for each n
			T[k] = MatMul(A, x, y, n);
		}

		free_matriz(A, n); free(x); free(y);



		mu = mean(T, V);
		sigma = stddev(T, V);

		fprintf(fp, "%d|%.16e+%.16e|%d\n", n, mu, sigma, V);
	}
	fclose(fp);

	return 0;
	}
