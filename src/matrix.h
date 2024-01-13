#ifndef MATRIX_H
#define MATRIX_H

#include "vector.h"

typedef Vector *Matrix;

Matrix mallocMatrix(const int dim);
void freeMatrix(Matrix m, const int dim);
void printMatrix(const Matrix m, const int dim);

#endif
