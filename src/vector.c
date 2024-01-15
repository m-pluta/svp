#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "vector.h"

Vector mallocVector(const int dim) {
    // Allocate space for `dim` elements in the vector
    Vector v = malloc(dim * sizeof(double));
    if (v == NULL) {
        printf("Failed to malloc Vector\n");
        return NULL;
    }
    return v;
}

void freeVector(Vector v) {
    free(v);
    v = NULL;
}

void printVector(const Vector v, const int dim) {
    printf("Vector: [");
    for (int i = 0; i < dim; ++i) {
        // Add spaces between all elements, except after the last one
        if (i != 0) {
            printf(" ");
        }
        printf("%8.4f", v[i]);
    }
    printf("]\n");
}

double inner_product(const Vector v1, const Vector v2, const int dim) {
    double total = 0;
    for (int i = 0; i < dim; i++) {
        total += v1[i] * v2[i];
    }
    return total;
}

double norm(const Vector v, const int dim) {
    // Calculates the L2, Euclidean norm
    return sqrt(inner_product(v, v, dim));
}
