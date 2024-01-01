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

void init_var(int dim, GS_Info *gs_info, double **p, int **v, double **c, int **w) {
    *p = calloc(dim + 1, sizeof(double));
    *v = calloc(dim, sizeof(int));
    *c = calloc(dim, sizeof(double));
    *w = calloc(dim, sizeof(int));
    if (gs_info == NULL || *p == NULL || *v == NULL || *c == NULL || *w == NULL) {
        printf("Failed to malloc gs_info, p, v, c, w");
        freeGSInfo(gs_info, dim);
        free(*p);
        free(*v);
        free(*c);
        free(*w);
        return;
    }
    (*v)[0] = 1;
}

double schorr_euchner(const int dim, GS_Info *gs_info, double R) {
    double *p, *c;
    int *v, *w;
    init_var(dim, gs_info, &p, &v, &c, &w);

    double R_2 = R * R;
    int k = 0;
    int last_non_zero = 0;

    double *inner_products = malloc(dim * sizeof(double));
    for (int i = 0; i < dim; i++) {
        inner_products[i] = inner_product(gs_info->Bs->v[i], gs_info->Bs->v[i], dim);
    }

    while(1) {
        p[k] = p[k+1] + ((v[k] - c[k]) * (v[k] - c[k])) * inner_products[k];

        if (p[k] < R_2) {
            if (k == 0) {
                R_2 = p[k];

            } else {
                k -= 1;
                c[k] = calculate_ck(gs_info->mu, dim, k, v);
                v[k] = (int) round(c[k]);
                w[k] = 1;
            }
        } else {
            k += 1;
            if (k == dim) {
                freeGSInfo(gs_info, dim);
                free(p);
                free(v);
                free(c);
                free(w);
                free(inner_products);
                // Print out the coefficients of the shortest vector found
                // printf("\n");
                // for (int i = 0; i < dim; i++) {
                //     printf("%3.1f ", v[i]);
                // }
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