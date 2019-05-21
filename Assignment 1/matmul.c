#include "matmul.h"
#include <string.h>

#ifndef USAGE
#define USAGE "usage:\n ./matmul [n] ([n] [1] [1]) \n\n ./matmul [n] [N] ([1] [1]) \n\n ./matmul [n] [N] [s] ([1]) \n\n ./matmul [n] [N] [s] [v]\n\n"
#endif

#ifndef BEGIN
#define BEGIN "running ./matmul [%d] [%d] [%d] [%d]\n"
#endif

#ifndef PRECISION
#define PRECISION 8
#endif

int
main(int argc, const char* argv[]){
    int seed = time(NULL);
    srand(seed);
	
	int n, N, s, V, ram;
	
	if (!strcmp(argv[1], "-h")){
		printf(USAGE);
		return 0;
	}
	
	switch(argc - 1){
        case 0:
            n = 2;
            N = 2;
            s = 1;
            V = 1;
            break;

		case 1:
			n = atoi(argv[1]);
			N = n;
			s = 1;
			V = 1;
			break;
		case 2:
			n = atoi(argv[1]);
			N = atoi(argv[2]);
			s = 1;
			V = 1;
			break;
		case 3:
			n = atoi(argv[1]);
			N = atoi(argv[2]);
			s = atoi(argv[3]);
			V = 1;
			break;
		case 4:
			n = atoi(argv[1]);
			N = atoi(argv[2]);
			s = atoi(argv[3]);
			V = atoi(argv[4]);
			break;
		case 5:
			n = atoi(argv[1]);
			N = atoi(argv[2]);
			s = atoi(argv[3]);
			V = atoi(argv[4]);
		default:
			printf(USAGE);
			return 1;
	}
	
	printf(BEGIN, n, N, s, V);
	
	if (n < 2) {
		printf("Warning: n must be 2 or greater. Assuming n = 2.\n\n");
	}
	if (N < n) {
		printf("Warning: N must be n or greater. Assuming N = n.\n\n");
	}

	FILE* fp_c;
	FILE* fp_f;

	fp_c = fopen("c_result_c.txt","w");
	fp_f = fopen("c_result_f.txt","w");

	double** A;
	double* x;
	double* y;

	double* T_c;
	double* T_f;

	double t_c;
	double t_f;

	int k;
	int v;
	
	int d = (N - n)/s;
	int i = 0;
	
	clock_t antes, depois;
	
	antes = clock();

	while(n <= N){ // iterate over matrix sizes
		v = sample(V, n);
		
		T_c = vetor(v);
		T_f = vetor(v);

		A = matriz(n, n);
        x = vetor(n);
        y = vetor(n);

		for(k=0; k<v; k++){ // samples for each n
			t_c = MatMulC(A, x, y, n);
			t_f = MatMulF(A, x, y, n);

			T_c[k] = t_c;
			T_f[k] = t_f;
			depois = clock();
			loading(i, d, depois-antes);
		}

		free_matriz(A, n); free(x); free(y);

		double mu_c, sigma_c;
		double mu_f, sigma_f;

		mu_c = mean(T_c, v); sigma_c = stddev(T_c, v);
		mu_f = mean(T_f, v); sigma_f = stddev(T_f, v);

		fprintf(fp_c, "%d|%.16e$%.16e|%d\n", n, mu_c, sigma_c, v);
		fprintf(fp_f, "%d|%.16e$%.16e|%d\n", n, mu_f, sigma_f, v);
		n += s;
		i++;
		
	}
	printf("\n-finished-\n");
	fclose(fp_c);
	fclose(fp_f);

	return 0;
	}
