#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    double *values;
} Vector;

Vector* mallocVector(int dimension) {
    Vector* vector = (Vector*)malloc(sizeof(Vector));
    if (vector == NULL) {
        return NULL;
    }
    vector->values = (double*)malloc(dimension * sizeof(double));
    if (vector->values == NULL) {
        free(vector);
        return NULL;
    }
    return vector;
}

void freeVector(Vector *vector) {
    free(vector->values);
    free(vector);
}

void printVector(const Vector *vector, int dimension) {
    printf("Vector: ");
    for (int i = 0; i < dimension; ++i) {
        printf("%.2f", vector->values[i]);
        if (i != dimension - 1) {
            printf(" ");
        }
    }
    printf("\n");
}

Vector* addVectors(const Vector *vector1, const Vector *vector2, const int dimension) {
    Vector* result = mallocVector(dimension);
    if (result == NULL) {
        return NULL;
    }
    for (int i = 0; i < dimension; i++) {
        result->values[i] = vector1->values[i] + vector2->values[i];
    }
    return result;
}

Vector* subVectors(const Vector *vector1, const Vector *vector2, const int dimension) {
    Vector* result = mallocVector(dimension);
    if (result == NULL) {
        return NULL;
    }
    for (int i = 0; i < dimension; i++) {
        result->values[i] = vector1->values[i] - vector2->values[i];
    }
    return result;
}

double inner_product(const Vector *vector1, const Vector *vector2, const int dimension) {
    double result = 0;
    for (int i = 0; i < dimension; i++) {
        result += vector1->values[i] * vector2->values[i];
    }
    return result;
}

double norm(const Vector *vector, const int dimension) {
    double innerProd = inner_product(vector, vector, dimension);
    return sqrt(innerProd);
}