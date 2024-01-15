#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "vector.h"
#include "matrix.h"
#include "gram_schmidt.h"

GS_Info *mallocGS_Info(const int dim) {
    // Malloc the struct
    GS_Info *gs_info = malloc(sizeof(GS_Info));
    if (gs_info == NULL) {
        printf("Failed to malloc Gram_Schmidt_Information\n");
        return NULL;
    }

    // Attempt to malloc the mu matrix
    gs_info->mu = mallocMatrix(dim);
    if (gs_info->mu == NULL) {
        free(gs_info);
        printf("Failed to malloc Matrix: mu\n");
        return NULL;
    }

    // Attempt to malloc the B-star matrix
    gs_info->Bs = mallocMatrix(dim);
    if (gs_info->Bs == NULL) {
        free(gs_info->mu);
        free(gs_info);
        printf("Failed to malloc Matrix: Bs\n");
        return NULL;
    }

    // Attempt to malloc the inner_products array for memoisation
    gs_info->inner_products = malloc(dim * sizeof(double));
    if (gs_info->inner_products == NULL) {
        free(gs_info->mu);
        free(gs_info->Bs);
        free(gs_info);
        printf("Failed to malloc inner products array\n");
        return NULL;
    }

    return gs_info;
}

void freeGSInfo(GS_Info *gs_info, const int dim) {
    freeMatrix(gs_info->mu, dim);
    freeMatrix(gs_info->Bs, dim);
    free(gs_info->inner_products);
    free(gs_info);
    gs_info = NULL;
}

void gram_schmidt(Matrix B, GS_Info *gs_info, const int dim) {
    // Extract mu and Bs for readability
    Matrix Bs = gs_info->Bs;
    Matrix mu = gs_info->mu;

    // Memoise the inner products throughout execution for performance
    double* inner_products = gs_info->inner_products;

    for (int i = 0; i < dim; i++) {
        // Copy Basis into Bs
        memcpy(Bs[i], B[i], dim * sizeof(double));

        // Orthogonalise the basis
        for (int k = 0; k < i; k++) {
            double ip = inner_product(B[i], Bs[k], dim);
            mu[i][k] = ip / inner_products[k];
            for (int j = 0; j < dim; j++) {
                Bs[i][j] -= mu[i][k] * Bs[k][j];
            }
        }

        // Store calculation of inner product for later computation
        inner_products[i] = inner_product(Bs[i], Bs[i], dim);
    }
}
