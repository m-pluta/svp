#ifndef VECTOR_H
#define VECTOR_H

typedef struct {
    double *e;
} Vector;

Vector* mallocVector(const int dim);
void freeVector(Vector *v);
void printVector(const Vector *v, const int dim);

Vector* addVectors(const Vector *v1, const Vector *v2, const int dim);
Vector* subVectors(const Vector *v1, const Vector *v2, const int dim);
double inner_product(const Vector *v1, const Vector *v2, const int dim);
double norm(const Vector *v, const int dim);

#endif // VECTOR_H