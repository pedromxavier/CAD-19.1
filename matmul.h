#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#ifndef LOAD_SIZE
#define LOAD_SIZE      20
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

double*
bigarray(int* n) {
	// returns a pointer for the biggest array possible
	(*n) = 1;
	double* x;
	double* y;
	
	do {
		x = y;
		(*n) <<= 1;
		y = (double*) malloc((*n)* sizeof(double));
		
	} while(y != nullptr);
	
	(*n) >>= 1;
	return x;
}

int
sample(int v, int n){
	int k = (int) v / ceil(log2(n));
	if(k >= 3){
		return k;
	}
	else{
		return 3;
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

double**
matriz(int m, int n){
    double** temp;
	temp = (double**) malloc(m*sizeof(double*));
	int i;
	for(i=0; i<m; i++){
        temp[i] = vetor(n);
	}
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
