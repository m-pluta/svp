#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "gram_schmidt.h"
#include "vector.h"
#include "matrix.h"

// Helper function to calculate ck = -sum_{k+1}^{dim}{mu[i][k] * v[i]}
double calculate_ck(Matrix mu, int dim, int k, int *v) {
    double ck = 0;
    for (int i = k + 1; i < dim; i++) {
        ck -= mu[i][k] * v[i];
    }
    return ck;
}

void init_var(int dim, double **p, int **v, double **c, int **w) {
    // Calloc all arrays
    *p = calloc(dim + 1, sizeof(double));
    *v = calloc(dim, sizeof(int));
    *c = calloc(dim, sizeof(double));
    *w = calloc(dim, sizeof(int));
    if (*p == NULL || *v == NULL || *c == NULL || *w == NULL) {
        printf("Failed to malloc p, v, c, w");
        free(*p);
        free(*v);
        free(*c);
        free(*w);
        return;
    }
    // Initially the SE solution is just the first vector
    *v[0] = 1;
}

double schorr_euchner(const int dim, const GS_Info *gs_info, double R_2) {
    // Extract B star and mu matrix for code readability
    Matrix Bs = gs_info->Bs;
    Matrix mu = gs_info->mu;

    // Declare pointers for arrays
    double *p, *c;
    int *v, *w;
    init_var(dim, &p, &v, &c, &w);

    int k = 0;
    int last_non_zero = 0;

    // Pre-calculate all the inner products - Performance
    double* inner_products = gs_info->inner_products;
    for (int i = 0; i < dim; i++) {
        inner_products[i] = inner_product(Bs[i], Bs[i], dim);
    }

    while (1) {
        p[k] = p[k + 1] + ((v[k] - c[k]) * (v[k] - c[k])) * inner_products[k];

        // R squared represents the search area
        if (p[k] < R_2) {
            if (k == 0) {
                // New shortest vector found
                R_2 = p[k];
            } else {
                k -= 1;
                c[k] = calculate_ck(mu, dim, k, v);
                v[k] = round(c[k]);
                w[k] = 1;
            }
        } else {
            k += 1;
            if (k == dim) {
                // Optimal solution found
                // Print out the coefficients of the shortest vector found
                // printf("\n");
                // for (int i = 0; i < dim; i++) {
                //     printf("%d ", v[i]);
                // }
                // printf("\n");

                free(p);
                free(v);
                free(c);
                free(w);
                return sqrt(R_2);
            }

            if (k >= last_non_zero) {
                last_non_zero = k;
                v[k] += 1;
            } else {
                // This is the zig-zag search that starts at v[k] and then
                // zig-zags +-1, +-2, +-3, etc to find a more optimal solution
                if (v[k] > c[k]) {
                    v[k] -= w[k];
                } else {
                    v[k] += w[k];
                }
                // Extend length of the zig-zag
                w[k] += 1;
            }
        }
    }
}
