#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "vector.h"
#include "matrix.h"
#include "gram_schmidt.h"

// Macro to calculate max of two values
#define max_int(x, y) (((x) >= (y)) ? (x) : (y))

// Size reduction of vector Bk
void size_reduce_bk(Matrix B, Matrix mu, const int dim, const int k)
{
    for (int j = k - 1; j >= 0; j--)
    {
        if (fabs(mu[k][j]) > 0.5)
        {
            __int64_t mu_rounded = (__int64_t)round(mu[k][j]);

            // Update Bk
            for (int i = 0; i < dim; i++)
            {
                B[k][i] -= mu_rounded * B[j][i];
            }

            // Update mu values without recomputing GS_info
            mu[k][j] -= mu_rounded;
            for (int i = 0; i < j; i++)
            {
                mu[k][i] -= mu_rounded * mu[j][i];
            }
        }
    }
}

void LLL(Matrix B, GS_Info *gs_info, const double delta, const int dim)
{
    // Extract B-star and mu matrices for code readability
    Matrix Bs = gs_info->Bs;
    Matrix mu = gs_info->mu;

    // Store certain inner-products
    long double inner_products[dim];

    // Stores if GS was recomputed on the previous iteration
    int first_iter = 1;

    int k = 1;
    while (k < dim)
    {
        size_reduce_bk(B, mu, dim, k);

        // This could be simplified however this is an optimisation,
        // In between gram_schmidt calls, the values of B-star do not change
        // This means ip_Bs_k on one iteration will be ip_Bs_k-1 on the next
        // because k increments
        long double ip_Bs_k = inner_product(Bs[k], Bs[k], dim);
        long double lovasz = (delta - (mu[k][k - 1]) * (mu[k][k - 1]));
        long double ip_Bs_k_1;
        if (first_iter == 1)
        {
            ip_Bs_k_1 = inner_product(Bs[k - 1], Bs[k - 1], dim);
            first_iter = 0;
        }
        else
        {
            ip_Bs_k_1 = inner_products[k - 1];
        }

        if (ip_Bs_k > lovasz * ip_Bs_k_1)
        {
            // Store ip for next iter
            inner_products[k] = ip_Bs_k;

            // Go to next vector in basis
            k += 1;
            break;
        }
        else
        {
            // Swap B_k and B_k-1
            Vector temp = B[k];
            B[k] = B[k - 1];
            B[k - 1] = temp;

            // Recompute GS_info
            gram_schmidt(B, gs_info, dim);

            // Signal that GS has been recomputed
            first_iter = 1;

            // Next k
            k = max_int(k - 1, 1);
        }
    }
}
