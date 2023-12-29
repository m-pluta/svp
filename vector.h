#ifndef VECTOR_H
#define VECTOR_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    double *values;
} Vector;

Vector* mallocVector(int dimension);
void freeVector(Vector *vector);
void printVector(const Vector *vector, int dimension);

#endif // VECTOR_H