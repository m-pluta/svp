#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct {
    double *e;
} Vector;

Vector* mallocVector(int dim) {
    Vector* v = (Vector*)malloc(sizeof(Vector));
    if (v == NULL) {
        return NULL;
    }
    v->e = (double*)malloc(dim * sizeof(double));
    if (v->e == NULL) {
        free(v);
        return NULL;
    }
    return v;
}

void freeVector(Vector *v) {
    free(v->e);
    free(v);
}

void printVector(const Vector *v, int dim) {
    printf("Vector: [");
    for (int i = 0; i < dim; ++i) {
        if (i != 0) {
            printf(" ");
        }
        printf("%.2f", v->e[i]);
    }
    printf("]\n");
}

Vector* addVectors(const Vector *v1, const Vector *v2, const int dim) {
    Vector* res = mallocVector(dim);
    if (res == NULL) {
        return NULL;
    }
    for (int i = 0; i < dim; i++) {
        res->e[i] = v1->e[i] + v2->e[i];
    }
    return res;
}

Vector* subVectors(const Vector *v1, const Vector *v2, const int dim) {
    Vector* res = mallocVector(dim);
    if (res == NULL) {
        return NULL;
    }
    for (int i = 0; i < dim; i++) {
        res->e[i] = v1->e[i] - v2->e[i];
    }
    return res;
}

double inner_product(const Vector *v1, const Vector *v2, const int dim) {
    double res = 0;
    for (int i = 0; i < dim; i++) {
        res += v1->e[i] * v2->e[i];
    }
    return res;
}

double norm(const Vector *v, const int dim) {
    return sqrt(inner_product(v, v, dim));
}