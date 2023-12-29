#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <vector.h>

typedef struct {
    Vector **vectors;
    int dimension;
} Basis;

Basis* mallocBasis(int dimension) {
    Basis* basis = (Basis*)malloc(sizeof(Basis));
    if (basis == NULL) {
        return NULL;
    }

    basis->vectors = (Vector**)malloc(dimension * sizeof(Vector*));
    if (basis->vectors == NULL) {
        free(basis);
        return NULL;
    }

    for (int i = 0; i < dimension; ++i) {
        basis->vectors[i] = mallocVector(dimension);
        if (basis->vectors[i] == NULL) {
            return NULL;
        }
    }

    basis->dimension = dimension;
    return basis;
}

void freeBasis(Basis *basis) {
    int i;

    for (i = 0; i < basis->dimension; ++i) {
        free(basis->vectors[i]->values);
        free(basis->vectors[i]);
    }

    free(basis->vectors);
}

void printBasis(const Basis *basis) {
    int i, j;

    printf("Basis Dimension: %d\n", basis->dimension);
    
    for (i = 0; i < basis->dimension; ++i) {
        printf("Vector %d: ", i + 1);
        for (j = 0; j < basis->dimension; ++j) {
            printf("%.2f ", basis->vectors[i]->values[j]);
        }
        printf("\n");
    }
}