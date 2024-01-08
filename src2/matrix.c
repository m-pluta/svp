#include <stdio.h>
#include <stdlib.h>

#include "vector.h"
#include "matrix.h"

Matrix mallocMatrix(const int dim) {
    Matrix m = malloc(dim * sizeof(Vector));
    if (m == NULL) {
        printf("Failed to malloc Matrix");
        return NULL;
    }

    for (int i = 0; i < dim; i++) {
        m[i] = mallocVector(dim);
        if (m[i] == NULL) {
            printf("Failed to malloc Matrix's inner vectors");
            for (int j = i - 1; j >= 0; j++) {
                freeVector(m[j]);
            }
            free(m);
            return NULL;
        }
    }
    return m;
}

void freeMatrix(Matrix m, const int dim) {
    for (int i = 0; i < dim; ++i) {
        freeVector(m[i]);
    }
    free(m);
    m = NULL;
}

void printMatrix(const Matrix m, const int dim) {
    printf("Matrix Dimension: %d\n", dim);

    for (int i = 0; i < dim; ++i) {
        printf("Vector %2.d: [", i + 1);
        for (int j = 0; j < dim; ++j) {
            if (j != 0) {
                printf(" ");
            }
            printf("%8.4f", m[i][j]);
        }
        printf("]\n");
    }
}
