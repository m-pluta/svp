#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    double *values;
} Vector;

typedef struct {
    Vector **vectors;
    int dimension;
} Basis;

Vector* mallocVector(int dimension);
void freeVector(Vector *vector);
void printVector(const Vector *vector, int dimension);

Basis* mallocBasis(int dimension);
void freeBasis(Basis *basis);
void printBasis(const Basis *basis);

int parseInput(Basis *basis, int num_args, char *args[]);
