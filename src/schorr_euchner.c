#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "gram_schmidt.h"
#include "vector.h"
#include "vector2d.h"
#include "bound.h"

double schorr_euchner(const Vector2D *B) {
    GS_Info *gs_info = gram_schmidt(B);

    double bound = lambda_1(gs_info->Bs);

    // printf("Bound: %.6f\n", bound);

    double *p = calloc(B->dim + 1, sizeof(double));
    double *v = calloc(B->dim, sizeof(double));
    double *c = calloc(B->dim, sizeof(double));
    double *w = calloc(B->dim, sizeof(double));
    if (gs_info == NULL || p == NULL || v == NULL || c == NULL || w == NULL) {
        printf("Failed to malloc gs_info, p, v, c, w");
        freeGSInfo(gs_info);
        free(p);
        free(v);
        free(c);
        free(w);
        return 1;
    }
    v[0] = 1;

    double R_2 = bound * bound;
    int k = 0;
    int last_non_zero = 0;

    for(;;) {
        p[k] = p[k+1] + ((v[k] - c[k]) * (v[k] - c[k])) * inner_product(gs_info->Bs->v[k], gs_info->Bs->v[k],B->dim);

        if (p[k] < R_2) {
            if (k == 0) {
                R_2 = p[k];

            } else {
                k -= 1;
                c[k] = 0;
                for (int i = k + 1; i < B->dim; i++) {
                    c[k] -= gs_info->mu->v[i]->e[k] * v[i];
                }
                v[k] = round(c[k]);
                w[k] = 1;
            }
        } else {
            k += 1;
            if (k == B->dim) {
                freeGSInfo(gs_info);
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