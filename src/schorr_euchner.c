#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "gram_schmidt.h"
#include "vector.h"
#include "vector2d.h"

double calculate_ck(Vector2D *mu, int dim, int k, int *v) {
    double result = 0;
    for (int i = k + 1; i < dim; i++) {
        result -= mu->v[i]->e[k] * v[i];
    }
    return result;
}

void init_var(int dim, double **p, int **v, double **c, int **w) {
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
    (*v)[0] = 1;
}

double schorr_euchner(const int dim, const GS_Info *gs_info, double R_2) {
    Vector2D *Bs = gs_info->Bs;
    Vector2D *mu = gs_info->mu;

    double *p, *c;
    int *v, *w;
    init_var(dim, &p, &v, &c, &w);

    int k = 0;
    int last_non_zero = 0;

    double *inner_products = malloc(dim * sizeof(double));
    for (int i = 0; i < dim; i++) {
        inner_products[i] = inner_product(Bs->v[i], Bs->v[i], dim);
    }

    while (1) {
        p[k] = p[k+1] + ((v[k] - c[k]) * (v[k] - c[k])) * inner_products[k];

        if (p[k] < R_2) {
            if (k == 0) {
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
                free(inner_products);
                free(p);
                free(v);
                free(c);
                free(w);
                // Print out the coefficients of the shortest vector found
                // printf("\n");
                // for (int i = 0; i < dim; i++) {
                //     printf("%d ", v[i]);
                // }
                // printf("\n");
                return sqrt(R_2);
            }

            if (k >= last_non_zero) {
                last_non_zero = k;
                v[k] += 1;
            } else {
                if (v[k] > c[k]) {
                    v[k] -= w[k];
                } else {
                    v[k] += w[k];
                }
                w[k] += 1;
            }
        }
    }
}
