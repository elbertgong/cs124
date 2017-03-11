// https://cs61.seas.harvard.edu/wiki/2016/caching-matrix has faster matmul libraries
// are we allowed to use all this cs61 stuff?

#define _GNU_SOURCE // for readline
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <assert.h>
#include <float.h>
#include <math.h>
#include <string.h>
#include <inttypes.h>
#include <getopt.h>
#include <sys/time.h>
#include <sys/types.h>

// crossover point
int n0 = 0;

double* a;
double* b;

// TODO: am I using size_t or int? Be consistent
// me(m, sz, i, j)
//    Return a pointer to matrix element `m[i][j]` -- the element
//    at row `i` and column `j`. The matrix is square with dimension
//    `sz`. Requires: `i < sz && j < sz`
static inline double* me(double* m, size_t sz, size_t i, size_t j) {
    return &m[i * sz + j];
}


// strassen(c, sz, a, b)
//    `a`, `b`, and `c` are square matrices with dimension `sz`.
//    Computes the matrix product `a x b` and stores it in `c`.
void strassen(double* c, size_t sz, double* a, double* b) {
	// TODO: actually implement this :D
    regular(c, sz, a, b);
}

// regular(c, sz, a, b)
//    `a`, `b`, and `c` are square matrices with dimension `sz`.
//    Computes the matrix product `a x b` and stores it in `c`.
void regular(double* c, size_t sz, double* a, double* b) {
    // clear `c`- get rid of this?
    for (size_t i = 0; i < sz; ++i)
        for (size_t j = 0; j < sz; ++j)
            *me(c, sz, i, j) = 0;

    // compute product and update `c`
    for (size_t i = 0; i < sz; ++i)
        for (size_t j = 0; j < sz; ++j)
            for (size_t k = 0; k < sz; ++k)
                *me(c, sz, i, j) += *me(a, sz, i, k) * *me(b, sz, k, j);
}

int main(int argc, char** argv) {
	if (argc != 4) {
		printf("Usage: strassen flag dimension inputfile\n");
		return 1;
	}
	int flag = atoi(argv[1]);
	size_t dim = atoi(argv[2]);
	char* inputfile = argv[3];
	char *line = NULL;
	size_t len = 0;
	ssize_t read;
    FILE* inptr = fopen(inputfile, "r");
    if (inptr == NULL)
    {
        printf("Could not open %s\n", inputfile);
        return 2;
    }

    // allocate matrices
    double* a = (double*) malloc(sizeof(double) * dim * dim);
    double* b = (double*) malloc(sizeof(double) * dim * dim);
    double* c = (double*) malloc(sizeof(double) * dim * dim);

    // read all lines of inputfile into a, b
    // currently am not checking validity of input
    for (size_t i = 0; i < dim; i++) {
    	for (size_t j = 0; j < dim; j++) {
    		if (getline(&line, &len, inptr) == -1) {
    			printf("read error\n");
    			return 1;
    		}
    		*me(c, dim, i, j) = atoi(line);
    	}
    }
    fclose(inptr);

    struct timeval time0, time1;
    gettimeofday(&time0, NULL);
    strassen(c, dim, a, b);
    gettimeofday(&time1, NULL);
    
    // print along the diagonal
    for (size_t i = 0; i < dim; ++i)
        printf("%f\n", *me(c, dim, i, i));
    
    // compute times, print times and ratio
    if (flag == 1) {
    	timersub(&time1, &time0, &time1);
    	printf("multiply time %ld.%06lds \n", time1.tv_sec, time1.tv_usec);
    }
    
    free(a);
    free(b);
    free(c);
  	return 0;
}