#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "vector.h"
#include "vector2d.h"
#include "gram_schmidt.h"

#define max_int(x,y) (((x) >= (y)) ? (x) : (y))

// Computes B_k = B_k - mu * B_j
void update_bk(Vector *B_k, const int mu, const Vector *B_j, const int dim) {
    for (int i = 0; i < dim; i++) {
        B_k->e[i] -= mu * B_j->e[i];
    }
}


void LLL(Vector2D *B, const int dim) {
    // Threshold
    float delta = 0.75;

    // Compute Initial GS_info
    GS_Info *gs_info = gram_schmidt(B, dim);

    double* inner_products = malloc(dim * sizeof(double));
    int first_iter = 1;

    int k = 1;
    while (k < dim) {
        for (int j = k - 1; j >= 0; j--) {
            double mu_rounded = (int) round(gs_info->mu->v[k]->e[j]);
            // If mu_rounded == 0, then skip pointless computation
            if (mu_rounded == 0) {
                continue;
            }

            // Update B_k
            if (fabs(gs_info->mu->v[k]->e[j]) > 0.5) {
                update_bk(B->v[k], mu_rounded, B->v[j], dim);
            }

            // Update mu values without recomputing GS_info
            gs_info->mu->v[k]->e[j] -= mu_rounded;
            for (int i = j - 1; i >= 0; i--) {
                gs_info->mu->v[k]->e[j] -= mu_rounded * gs_info->mu->v[k - 1]->e[j];
            }
            
        }

        double ip_Bs_k = inner_product(gs_info->Bs->v[k], gs_info->Bs->v[k], dim);
        double lovasz = (delta - (gs_info->mu->v[k]->e[k-1]) * (gs_info->mu->v[k]->e[k-1]));
        double ip_Bs_k_1;
        if (first_iter == 1) {
            ip_Bs_k_1 = inner_product(gs_info->Bs->v[k-1], gs_info->Bs->v[k-1], dim);
            first_iter = 0;
        } else {
            ip_Bs_k_1 = inner_products[k-1];
        }

        if (ip_Bs_k > lovasz * ip_Bs_k_1) {
            // Store ip for next iter
            inner_products[k] = ip_Bs_k;
            
            // Go to next vector in basis
            k += 1;
        } else {
            // Swap B_k and B_k-1
            Vector *temp = B->v[k];
            B->v[k] = B->v[k-1];
            B->v[k-1] = temp;

            // Recompute GS_info
            freeGSInfo(gs_info, dim);
            gs_info = gram_schmidt(B, dim);

            // Signal that GS has been recomputed
            first_iter = 1;

            // Next k
            k = max_int(k-1, 1);
        }
    }

    free(inner_products);
    freeGSInfo(gs_info, dim);
}