#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "vector.h"
#include "vector2d.h"
#include "gram_schmidt.h"

#define max_int(x, y) (((x) >= (y)) ? (x) : (y))

void size_reduce_bk(Vector2D *B, Vector2D *mu, const int dim, const int k) {
    for (int j = k - 1; j >= 0; j--) {
        if (fabs(mu->v[k]->e[j]) > 0.5) {
            __int64_t mu_rounded = (__int64_t) round(mu->v[k]->e[j]);

            // Update bk
            for (int i = 0; i < dim; i++) {
                B->v[k]->e[i] -= mu_rounded * B->v[j]->e[i];
            }

            // Update mu values without recomputing GS_info
            mu->v[k]->e[j] -= mu_rounded;
            for (int i = 0; i < j; i++) {
                mu->v[k]->e[i] -= mu_rounded * mu->v[j]->e[i];
            }
        }
    }
}


void LLL(Vector2D *B, GS_Info *gs_info, const double delta, const int dim) {
    Vector2D *Bs = gs_info->Bs;
    Vector2D *mu = gs_info->mu;

    long double inner_products[dim];
    int first_iter = 1;

    int k = 1;
    while (k < dim) {
        size_reduce_bk(B, mu, dim, k);

        long double ip_Bs_k = inner_product(Bs->v[k], Bs->v[k], dim);
        long double lovasz = (delta - (mu->v[k]->e[k-1]) * (mu->v[k]->e[k-1]));
        long double ip_Bs_k_1;
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
