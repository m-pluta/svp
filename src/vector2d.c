#include <stdio.h>
#include <stdlib.h>

#include "vector.h"

typedef struct {
    Vector **v;
    int dim;
} Vector2D;

Vector2D* mallocVector2D(int dim) {
    Vector2D* v2d = (Vector2D*)malloc(sizeof(Vector2D));
    if (v2d == NULL) {
        printf("Failed to malloc Vector2D");
        return NULL;
    }

    v2d->v = (Vector**)malloc(dim * sizeof(Vector*));
    if (v2d->v == NULL) {
        printf("Failed to malloc Vector2D's inner vectors");
        free(v2d);
        return NULL;
    }

    for (int i = 0; i < dim; ++i) {
        v2d->v[i] = mallocVector(dim);
        if (v2d->v[i] == NULL) {
            printf("Failed to malloc Vector2D's inner vectors's values");
            return NULL;
        }
    }

    v2d->dim = dim;
    return v2d;
}

void freeVector2D(Vector2D *v2d) {
    for (int i = 0; i < v2d->dim; ++i) {
        freeVector(v2d->v[i]);
    }
    free(v2d->v);
    free(v2d);
}

void printVector2D(const Vector2D *v2d) {
    printf("Vector2D Dimension: %d\n", v2d->dim);
    
    for (int i = 0; i < v2d->dim; ++i) {
        printf("Vector %d: [", i + 1);
        for (int j = 0; j < v2d->dim; ++j) {
            if (j != 0) {
                printf(" ");
            }
            printf("%.2f", v2d->v[i]->e[j]);
        }
        printf("]\n");
    }
}