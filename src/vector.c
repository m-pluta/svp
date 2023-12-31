#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct {
    double *e;
} Vector;

Vector* mallocVector(int dim) {
    Vector* v = (Vector*)malloc(sizeof(Vector));
    if (v == NULL) {
        printf("Failed to malloc Vector");
        return NULL;
    }
    v->e = (double*)malloc(dim * sizeof(double));
    if (v->e == NULL) {
        printf("Failed to malloc Vector values");
        free(v);
        return NULL;
    }
    return v;
}

void freeVector(Vector *v) {
    free(v->e);
    free(v);
}

void printVector(Vector *v, int dim) {
    printf("Vector: [");
    for (int i = 0; i < dim; ++i) {
        if (i != 0) {
            printf(" ");
        }
        printf("%8.4f", v->e[i]);
    }
    printf("]\n");
}

Vector* addVectors(Vector *v1, Vector *v2, int dim) {
    Vector* res = mallocVector(dim);
    if (res == NULL) {
        return NULL;
    }
    for (int i = 0; i < dim; i++) {
        res->e[i] = v1->e[i] + v2->e[i];
    }
    return res;
}

Vector* subVectors(Vector *v1, Vector *v2, int dim) {
    Vector* res = mallocVector(dim);
    if (res == NULL) {
        return NULL;
    }
    for (int i = 0; i < dim; i++) {
        res->e[i] = v1->e[i] - v2->e[i];
    }
    return res;
}

double inner_product(Vector *v1, Vector *v2, int dim) {
    double res = 0;
    for (int i = 0; i < dim; i++) {
        res += v1->e[i] * v2->e[i];
    }
    return res;
}

double norm(Vector *v, int dim) {
    return sqrt(inner_product(v, v, dim));
}