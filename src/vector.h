#ifndef VECTOR_H
#define VECTOR_H

typedef struct {
    double *e;
} Vector;

Vector* mallocVector(int dim);
void freeVector(Vector *v);
void printVector(Vector *v, int dim);

Vector* addVectors(Vector *v1, Vector *v2, int dim);
Vector* subVectors(Vector *v1, Vector *v2, int dim);
double inner_product(Vector *v1, Vector *v2, int dim);
double norm(Vector *v, int dim);

#endif // VECTOR_H