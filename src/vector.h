#ifndef VECTOR_H
#define VECTOR_H

typedef double *Vector;

Vector mallocVector(const int dim);
void freeVector(Vector v);
void printVector(const Vector v, const int dim);

double inner_product(const Vector v1, const Vector v2, const int dim);
double norm(const Vector v, const int dim);

#endif
