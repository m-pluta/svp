#ifndef VECTOR_H
#define VECTOR_H

typedef struct {
    double *values;
} Vector;

Vector* mallocVector(int dimension);
void freeVector(Vector *vector);
void printVector(const Vector *vector, int dimension);

Vector* addVectors(const Vector *vector1, const Vector *vector2, const int dimension);
Vector* subVectors(const Vector *vector1, const Vector *vector2, const int dimension);
double inner_product(const Vector *vector1, const Vector *vector2, const int dimension);
double norm(const Vector *vector, const int dimension);

#endif // VECTOR_H