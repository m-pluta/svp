#include <stdio.h>
#include <stdlib.h>

#include "vector.h"
#include "matrix.h"
#include "gram_schmidt.h"

void freeGSInfo(GS_Info *gs_info, const int dim)
{
    freeMatrix(gs_info->mu, dim);
    freeMatrix(gs_info->Bs, dim);
    free(gs_info);
}

GS_Info *mallocGS_Info(const int dim)
{
    GS_Info *gs_info = malloc(sizeof(GS_Info));
    if (gs_info == NULL)
    {
        printf("Failed to malloc Gram_Schmidt_Information");
        return NULL;
    }

    gs_info->mu = mallocMatrix(dim);
    if (gs_info->mu == NULL)
    {
        free(gs_info);
        printf("Failed to malloc Matrix: mu");
        return NULL;
    }

    gs_info->Bs = mallocMatrix(dim);
    if (gs_info->Bs == NULL)
    {
        free(gs_info->mu);
        free(gs_info);
        printf("Failed to malloc Matrix: Bs");
        return NULL;
    }
    return gs_info;
}

void gram_schmidt(Matrix B, GS_Info *gs_info, const int dim)
{
    Matrix Bs = gs_info->Bs;
    Matrix mu = gs_info->mu;
    double inner_products[dim];

    for (int i = 0; i < dim; i++)
    {
        for (int j = 0; j < dim; j++)
        {
            Bs[i][j] = B[i][j];
        }

        for (int k = 0; k < i; k++)
        {
            double ip = inner_product(B[i], Bs[k], dim);
            mu[i][k] = ip / inner_products[k];
            for (int j = 0; j < dim; j++)
            {
                Bs[i][j] -= mu[i][k] * Bs[k][j];
            }
        }

        // Store calculation of inner product for later computations
        inner_products[i] = inner_product(Bs[i], Bs[i], dim);
    }
}
