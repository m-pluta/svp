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