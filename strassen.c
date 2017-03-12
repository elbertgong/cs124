/* https://cs61.seas.harvard.edu/wiki/2016/caching-matrix has faster matmul libraries
most of this is based on cs61
are we allowed to use all this cs61 stuff? */

#define _GNU_SOURCE // for readline fn
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

int* a;
int* b;

// printmat(m, dim)
//    Print out a matrix for debugging
void printmat(int* m, int dim) {
	for (int i = 0; i < dim * dim; i++) {
		printf("%d ", *(m + i));
	}
	printf("\n");
}

// me(m, dim, i, j)
//    Return a pointer to matrix element `m[i][j]` -- the element
//    at row `i` and column `j`. The matrix is square with dimension
//    `dim`. Requires: `i < dim && j < dim`
static inline int* me(int* m, int dim, int i, int j) {
    return &m[i * dim + j];
}

// regular(c, dim, a, b)
//    `a`, `b`, and `c` are square matrices with dimension `dim`.
//    Computes the matrix product `a x b` and stores it in `c`.
void regular(int* c, int dim, int* a, int* b) {
    // clear `c`- get rid of this?
    for (int i = 0; i < dim; ++i)
        for (int j = 0; j < dim; ++j)
            *me(c, dim, i, j) = 0;

    // compute product and update `c`
    for (int i = 0; i < dim; ++i)
        for (int j = 0; j < dim; ++j)
            for (int k = 0; k < dim; ++k)
                *me(c, dim, i, j) += *me(a, dim, i, k) * *me(b, dim, k, j);
}

// strassen(c, dim, a, b)
//    `a`, `b`, and `c` are square matrices with dimension `dim`.
//    Computes the matrix product `a x b` and stores it in `c`.
void strassen(int* c, int dim, int* a, int* b) {
	// TODO: actually implement this :D
    regular(c, dim, a, b);
}

int main(int argc, char** argv) {
	if (argc != 4) {
		printf("Usage: strassen flag dimension inputfile\n");
		return 1;
	}
	int flag = atoi(argv[1]);
	int dim = atoi(argv[2]);
	char* inputfile = argv[3];
	char *line = NULL;
	size_t len = 0;
    FILE* inptr = fopen(inputfile, "r");
    if (inptr == NULL)
    {
        printf("Could not open %s\n", inputfile);
        return 2;
    }

    // allocate matrices
    int* a = (int*) malloc(sizeof(int) * dim * dim);
    int* b = (int*) malloc(sizeof(int) * dim * dim);
    int* c = (int*) malloc(sizeof(int) * dim * dim);

    // read all lines of inputfile into a, b
    // currently am not checking validity of input
    for (int i = 0; i < dim; i++) {
    	for (int j = 0; j < dim; j++) {
    		if (getline(&line, &len, inptr) == -1) {
    			printf("read error\n");
    			return 1;
    		}
    		*me(a, dim, i, j) = atoi(line);
    	}
    }
    for (int i = 0; i < dim; i++) {
    	for (int j = 0; j < dim; j++) {
    		if (getline(&line, &len, inptr) == -1) {
    			printf("read error\n");
    			return 1;
    		}
    		*me(b, dim, i, j) = atoi(line);
    	}
    }
    fclose(inptr);

    struct timeval time0, time1;
    gettimeofday(&time0, NULL);
    strassen(c, dim, a, b);
    gettimeofday(&time1, NULL);

    // print along the diagonal
    for (int i = 0; i < dim; ++i)
        printf("%d\n", *me(c, dim, i, i));
    
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