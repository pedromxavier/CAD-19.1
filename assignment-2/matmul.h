#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <omp.h>

#ifndef MIN_SAMPLE
#define MIN_SAMPLE     3
#endif


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
max_array_size(){
    return 2048;
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

void reset(double** A, double* x, double* y, int n){
    random_matriz(A, n, n);
	random_vetor(x, n);
	zero_vector(y, n);
}

double
dot(double* x, double* y, int n){
    int j; double s = 0.0;
    // #pragma omp for
    // {
        for(j=0;j<n;j++){
            s += x[j]*y[j];
        }
    // }
    return s;
}

double
MatMul(double** A, double* x, double*y, int n){
	reset(A, x, y, n);
	double t = omp_get_wtime();
	#pragma omp parallel
    {
        int i;
        #pragma omp for schedule(guided, 1)
        for(i = 0; i < n; i++){
            y[i] = dot(A[i], x, n);
        }
    }
    return omp_get_wtime() - t;
}
