#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#ifndef LOAD_SIZE
#define LOAD_SIZE      50
#endif

#ifndef MIN_SAMPLE
#define MIN_SAMPLE     3
#endif

#ifndef nullptr
#define nullptr     NULL
#endif

double
seconds(clock_t t){
	return ((double) t) / CLOCKS_PER_SEC;
}

void
loading(int i, int d, clock_t t){
	float I = (float) i;
	float D = (float) d;
	int m = (int) LOAD_SIZE*(I / D);
	
	int j, k;
	printf("|");
	
	for(j=0; j <= m; j++) {
		printf("#");
	}
	
	for(k=m; k < LOAD_SIZE; k++){
		printf(" ");
	}
	printf("|");
	
	int time = (int) (seconds(t));
	printf(" time: %d s", time);
	printf("\r");
}

double 
log2(double n){  
	return log(n) / log(2);  
}  

double
random_double(void){
	return (double)rand()/RAND_MAX;
}

double
mean(double* x, int n){
	double s = 0.0;
	int i;
	for(i=0; i<n; i++){
		s += x[i];
	}
	return s/n;
}

int
sample(int v, int n){
	int k = (int) v / (2*(ceil(log2(n)))-1);
	if(k >= MIN_SAMPLE){
		return k;
	}
	else{
		return MIN_SAMPLE;
	}
}

double
stddev(double*x, int n){
    if (n>1){
        double s = 0.0;
        double X = mean(x, n);
        int i;
        for(i=0; i<n; i++){
            s += pow(x[i]-X,2);
        }
        return sqrt(s/(n-1));
    }
    else{
        return 0.0;
    }

}

void
random_vetor(double* x, int n){
	int i;
	for(i=0; i<n; i++){
		x[i] = random_double();
	}
}

double*
vetor(int n){
	double* temp;
	temp = (double*) malloc(n*sizeof(double));
	return temp;
}

void
random_matriz(double** A, int m, int n){
    int i,j;
	for(i=0; i<m; i++){
        for (j=0; j<n;j++){
            A[i][j] = random_double();
        }
	}
}

void
zero_vector(double* y, int n){
    int i;
    for (i=0; i<n; i++){
        y[i] = 0;
    }
}

void
free_matriz(double** A, int n){
    int i;
    for(i=0;i<n;i++){
        free(A[i]);
    }
    free(A);
}

double**
matriz(int m, int n){
	 double** temp;
	temp = (double**) malloc(m*sizeof(double*));
	int i;
	for(i=0; i<m; i++){
		temp[i] = vetor(n);
		if(temp[i] == nullptr){
			free_matriz(temp, i);
			temp = nullptr; 
			return temp;
		}
	}
	return temp;
}

double
MatMulC(double** A, double* x, double*y, int n){
	int i,j;

	random_matriz(A, n, n);
	random_vetor(x, n);
	zero_vector(y, n);

	clock_t antes, depois;

	antes = clock();
	for(i=0;i<n;i++){
		for(j=0;j<n;j++){
			y[i] += A[i][j]*x[j];
		}
	}
	depois = clock();
    return seconds(depois - antes);
}

double
MatMulF(double** A, double* x, double*y, int n){
	int i,j;

	random_matriz(A, n, n);
	random_vetor(x, n);
	zero_vector(y, n);

	clock_t antes, depois;
	antes = clock();
	for(j=0;j<n;j++){
		for(i=0;i<n;i++){
			y[i] += A[i][j]*x[j];
		}
	}
	depois = clock();
    return ((double)(depois-antes))/CLOCKS_PER_SEC;
}

int
max_array_size(void) {
	// returns the maximum N to allocate A, x and y
	int n = 1;
	double** A = matriz(n, n);
	double* x = vetor(n);
	double* y = vetor(n);
	
	while(1) {	
		free_matriz(A, n); free(x); free(y);

		n <<= 1;

		A = matriz(n, n);
		if (A == nullptr){
			break;
		} 
		
		x = vetor(n);
		if (x == nullptr) {
			free_matriz(A, n);
			break;
		}
		
		y = vetor(n);
		if (y == nullptr) {
			free_matriz(A, n);
			free(x);
			break;
		}	
	}
	return n >> 1;
}
