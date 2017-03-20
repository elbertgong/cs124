/* https://cs61.seas.harvard.edu/wiki/2016/caching-matrix has faster matmul libraries
most of this is based on cs61
OH Questions:
are we allowed to use all this cs61 stuff? 
Which size matrix are we gonna use for testing the crossover pt?
What they're asking for implies a pretty set crossover pt?
Must we have o many (17) allocations per iteration?
should initial malloc of c and components(fh, p1, etc) be callocs?
TODO:
Describe procedure.
Implement mmap?
Test analytically for crossover point.
Create graphs and writeup of analytical thing.
*/

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
int n0 = 1;

int* a;
int* b;
int* c_addr;

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

// padzeros(m, num)
//    Pads 'num' rows and 'num' columns of zeros onto matrix 'm' with dimension 'dim'
int* padzeros(int* m, int dim, int num) {
    int* mtemp = (int*) calloc((dim + num) * (dim + num), sizeof(int));
    for (int i = 0; i < dim; i++) {
        for (int j = 0; j < dim; j++) {
            *me(mtemp, dim + num, i, j) = *me(m, dim, i, j);
        }
    }
    return mtemp;
}

// add(a, ax, ay, b, bx, by, c, cx, cy, dim)
// Does c=a+b, with the offsets given by the x's and y's
void add(int* a, int ax, int ay, int* b, int bx, int by, int* c, int cx, int cy, int adim, int bdim, int cdim) {
    int dim = (adim < bdim) ? adim : bdim;
    dim = (cdim < dim) ? cdim : dim;
    for (int i = 0; i < dim; i++) {
        for (int j = 0; j < dim; j++) {
            *me(c, cdim, cx+i, cy+j) = *me(a, adim, ax+i, ay+j) + *me(b, bdim, bx+i, by+j);
        }
    }
}

// sub(a, ax, ay, b, bx, by, c, cx, cy, dim)
// Does c=a-b, with the offsets given by the x's and y's
void sub(int* a, int ax, int ay, int* b, int bx, int by, int* c, int cx, int cy, int adim, int bdim, int cdim) {
    int dim = (adim < bdim) ? adim : bdim;
    dim = (cdim < dim) ? cdim : dim;
    for (int i = 0; i < dim; i++) {
        for (int j = 0; j < dim; j++) {
            // printf("taking %d - %d\n", *me(a, abdim, ax+i, ay+j), *me(b, abdim, bx+i, by+j));
            *me(c, cdim, cx+i, cy+j) = *me(a, adim, ax+i, ay+j) - *me(b, bdim, bx+i, by+j);
        }
    }
}

// strassen(c, dim, a, b)
//    `a`, `b`, and `c` are square matrices with dimension `dim`.
//    Computes the matrix product `a x b` and stores it in `c`.
void strassen(int* c, int dim, int* a, int* b) {
    if (dim <= n0) {
        regular(c, dim, a, b);
        return;
    }
    
    // we shouldn't do this more than once
    double roundup = pow(2, ceil(log(dim)/log(2.0)));
    int newdim = (int) roundup;
    if (dim != newdim) {
        a = padzeros(a, dim, newdim - dim);
        b = padzeros(b, dim, newdim - dim);
        c = padzeros(c, dim, newdim - dim);
        c_addr = c;
    }

    int x = newdim / 2;
    int* atemp = (int*) malloc(sizeof(int) * x * x);
    int* htemp = (int*) malloc(sizeof(int) * x * x);
    int* etemp = (int*) malloc(sizeof(int) * x * x);
    int* dtemp = (int*) malloc(sizeof(int) * x * x);
    for (int i = 0; i < x; i++) {
        for (int j = 0; j < x; j++) {
            *me(atemp, x, i, j) = *me(a, newdim, i, j);
            *me(htemp, x, i, j) = *me(b, newdim, i+x, j+x);
            *me(etemp, x, i, j) = *me(b, newdim, i, j);
            *me(dtemp, x, i, j) = *me(a, newdim, i+x, j+x);
        }
    }

    int* fh = (int*) malloc(sizeof(int) * x * x);
    sub(b, 0, x, b, x, x, fh, 0, 0, newdim, newdim, x);
    int* ab = (int*) malloc(sizeof(int) * x * x);
    add(a, 0, 0, a, 0, x, ab, 0, 0, newdim, newdim, x);
    int* cd = (int*) malloc(sizeof(int) * x * x);
    add(a, x, 0, a, x, x, cd, 0, 0, newdim, newdim, x);
    int* ge = (int*) malloc(sizeof(int) * x * x);
    sub(b, x, 0, b, 0, 0, ge, 0, 0, newdim, newdim, x);
    int* ad = (int*) malloc(sizeof(int) * x * x);
    add(a, 0, 0, a, x, x, ad, 0, 0, newdim, newdim, x);
    int* eh = (int*) malloc(sizeof(int) * x * x);
    add(b, 0, 0, b, x, x, eh, 0, 0, newdim, newdim, x);
    int* bd = (int*) malloc(sizeof(int) * x * x);
    sub(a, 0, x, a, x, x, bd, 0, 0, newdim, newdim, x);
    int* gh = (int*) malloc(sizeof(int) * x * x);
    add(b, x, 0, b, x, x, gh, 0, 0, newdim, newdim, x);
    int* ac = (int*) malloc(sizeof(int) * x * x);
    sub(a, 0, 0, a, x, 0, ac, 0, 0, newdim, newdim, x);
    int* ef = (int*) malloc(sizeof(int) * x * x);
    add(b, 0, 0, b, 0, x, ef, 0, 0, newdim, newdim, x);
    
    /* printf("intermediary steps below\n");
    printmat(atemp, x);
    printmat(fh, x);
    printmat(ab, x);
    printmat(htemp, x);
    printmat(cd, x);
    printmat(etemp, x);
    printmat(dtemp, x);
    printmat(ge, x);
    printmat(ad, x);
    printmat(eh, x);
    printmat(bd, x);
    printmat(gh, x);
    printmat(ac, x);
    printmat(ef, x); */
    
    int* p1 = (int*) malloc(sizeof(int) * x * x);
    strassen(p1, x, atemp, fh);
    int* p2 = (int*) malloc(sizeof(int) * x * x);
    strassen(p2, x, ab, htemp);
    int* p3 = (int*) malloc(sizeof(int) * x * x);
    strassen(p3, x, cd, etemp);
    int* p4 = (int*) malloc(sizeof(int) * x * x);
    strassen(p4, x, dtemp, ge);
    int* p5 = (int*) malloc(sizeof(int) * x * x);
    strassen(p5, x, ad, eh);
    int* p6 = (int*) malloc(sizeof(int) * x * x);
    strassen(p6, x, bd, gh);
    int* p7 = (int*) malloc(sizeof(int) * x * x);
    strassen(p7, x, ac, ef);
    
    /* printf("p intermediary steps below\n");
    printmat(p1, x);
    printmat(p2, x);
    printmat(p3, x);
    printmat(p4, x);
    printmat(p5, x);
    printmat(p6, x);
    printmat(p7, x); */
    
    add(p4, 0, 0, p5, 0, 0, c, 0, 0, x, x, newdim);
    sub(c, 0, 0, p2, 0, 0, c, 0, 0, newdim, x, newdim);
    add(c, 0, 0, p6, 0, 0, c, 0, 0, newdim, x, newdim);

    add(p1, 0, 0, p2, 0, 0, c, 0, x, x, x, newdim);
    
    add(p3, 0, 0, p4, 0, 0, c, x, 0, x, x, newdim);
    
    add(p1, 0, 0, p5, 0, 0, c, x, x, x, x, newdim);
    sub(c, x, x, p3, 0, 0, c, x, x, newdim, x, newdim);
    sub(c, x, x, p7, 0, 0, c, x, x, newdim, x, newdim);
    
    free(atemp);
    free(htemp);
    free(etemp);
    free(dtemp);
    free(fh);
    free(ab);
    free(cd);
    free(ge);
    free(ad);
    free(eh);
    free(bd);
    free(gh);
    free(ac);
    free(ef);
    free(p1);
    free(p2);
    free(p3);
    free(p4);
    free(p5);
    free(p6);
    free(p7);

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
    /* FILE* inptr = fopen(inputfile, "r");
    if (inptr == NULL)
    {
        printf("Could not open %s\n", inputfile);
        return 2;
    } */


    // allocate matrices
    int* a = (int*) malloc(sizeof(int) * dim * dim);
    int* b = (int*) malloc(sizeof(int) * dim * dim);
    int* c = (int*) malloc(sizeof(int) * dim * dim);
    c_addr = c;

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
    
    // go along the diagonal
    double roundup = pow(2, ceil(log(dim)/log(2.0)));
    int newdim = (int) roundup;
    for (int i = 0; i < dim; ++i)
        printf("%d\n", *me(c_addr, newdim, i, i));
    
    
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