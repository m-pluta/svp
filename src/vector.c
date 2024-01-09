#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "vector.h"

Vector mallocVector(const int dim)
{
    Vector v = malloc(dim * sizeof(double));
    if (v == NULL)
    {
        printf("Failed to malloc Vector");
        return NULL;
    }
    return v;
}

void freeVector(Vector v)
{
    free(v);
    v = NULL;
}

void printVector(const Vector v, const int dim)
{
    printf("Vector: [");
    for (int i = 0; i < dim; ++i)
    {
        if (i != 0)
        {
            printf(" ");
        }
        printf("%8.4f", v[i]);
    }
    printf("]\n");
}

double inner_product(const Vector v1, const Vector v2, const int dim)
{
    double res = 0;
    for (int i = 0; i < dim; i++)
    {
        res += v1[i] * v2[i];
    }
    return res;
}

double norm(const Vector v, const int dim)
{
    return sqrt(inner_product(v, v, dim));
}
