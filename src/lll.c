#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "vector.h"
#include "vector2d.h"
#include "gram_schmidt.h"

#define max_int(x,y) (((x) >= (y)) ? (x) : (y))

// Computes B_k = B_k - mu * B_j
void update_bk(Vector *B_k, const long long mu, const Vector *B_j, const int dim) {
    for (int i = 0; i < dim; i++) {
        B_k->e[i] -= mu * B_j->e[i];
    }
}

void size_reduce_bk(Vector2D *B, Vector2D *mu, const int dim, const int k) {
    for (int j = k - 1; j >= 0; j--) {
        if (fabs(mu->v[k]->e[j]) > 0.5) {
            long long mu_rounded = (long long int) round(mu->v[k]->e[j]);
            
            update_bk(B->v[k], mu_rounded, B->v[j], dim);

            // Update mu values without recomputing GS_info
            mu->v[k]->e[j] -= mu_rounded;
            for (int i = 0; i < j; i++) {
                mu->v[k]->e[i] -= mu_rounded * mu->v[j]->e[i];
            }
        }
    }
}


void LLL(Vector2D *B, GS_Info *gs_info, const int dim) {
    Vector2D *Bs = gs_info->Bs;
    Vector2D *mu = gs_info->mu;
    
    // Threshold
    float delta = 0.99;

    double inner_products[dim];
    int first_iter = 1;

    int k = 1;
    while (k < dim) {
        size_reduce_bk(B, mu, dim, k);

        double ip_Bs_k = inner_product(Bs->v[k], Bs->v[k], dim);
        double lovasz = (delta - (mu->v[k]->e[k-1]) * (mu->v[k]->e[k-1]));
        double ip_Bs_k_1;
        if (first_iter == 1) {
            ip_Bs_k_1 = inner_product(Bs->v[k-1], Bs->v[k-1], dim);
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
            gram_schmidt_in_place(B, gs_info, dim);

            // Signal that GS has been recomputed
            first_iter = 1;

            // Next k
            k = max_int(k-1, 1);
        }
    }
}