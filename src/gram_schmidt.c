#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "vector.h"
#include "matrix.h"
#include "gram_schmidt.h"

GS_Info *mallocGS_Info(const int dim)
{
    // Malloc the struct
    GS_Info *gs_info = malloc(sizeof(GS_Info));
    if (gs_info == NULL)
    {
        printf("Failed to malloc Gram_Schmidt_Information");
        return NULL;
    }

    // Attempt to malloc the mu matrix
    gs_info->mu = mallocMatrix(dim);
    if (gs_info->mu == NULL)
    {
        free(gs_info);
        printf("Failed to malloc Matrix: mu");
        return NULL;
    }

    // Attempt to malloc the B-star matrix
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

void freeGSInfo(GS_Info *gs_info, const int dim)
{
    freeMatrix(gs_info->mu, dim);
    freeMatrix(gs_info->Bs, dim);
    free(gs_info);
    gs_info = NULL;
}

void gram_schmidt(Matrix B, GS_Info *gs_info, const int dim)
{
    // Extract mu and Bs for readability
    Matrix Bs = gs_info->Bs;
    Matrix mu = gs_info->mu;

    // Memoise the inner products throughout execution for performance
    double inner_products[dim];

    for (int i = 0; i < dim; i++)
    {
        // Copy Basis into Bs
        memcpy(Bs[i], B[i], dim * sizeof(double)); 
        // Orthogonalise the basis
        for (int k = 0; k < i; k++)
        {
            printVector(B[1], dim);
            printVector(Bs[0], dim);
            printVector(Bs[1], dim);
            printf("i=%d, k=%d\n", i, k);
            double ip = inner_product(B[i], Bs[k], dim);
            mu[i][k] = ip / inner_products[k];
            // printVector(B[i], dim);
            // printVector(Bs[k], dim);
            for (int j = 0; j < dim; j++){
                printf("mu[i][k]           =%.60f\n", mu[i][k]);
                printf("Bs[k][j]           =%.60f\n", Bs[k][j]);
                printf("Bs[i][j]           =%.60f\n", Bs[i][j]);
                if (Bs[i][j] == 3763617828)
                printf("mu[i][k] * Bs[k][j]=%.60f\n", mu[i][k] * Bs[k][j]);
                // if (Bs[i][j] == mu[i][k] * Bs[k][j]) {
                //     printf("THEY ARE THE SAME\n");
                // }
                double test = Bs[i][j] - (mu[i][k] * Bs[k][j]);
                if (test == 0) {
                    printf("test is zero\n");
                }
                printf("test               =%.60f\n", test);
                Bs[i][j] = test;
                printf("Bs[i][j]           =%.60f\n\n", Bs[i][j]);
            }
        }

        // Store calculation of inner product for later computation
        inner_products[i] = inner_product(Bs[i], Bs[i], dim);
    }
}
