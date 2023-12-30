#include <math.h>
#include <stdio.h>

#include "vector.h"
#include "vector2d.h"
#include "gram_schmidt.h"

#define max_int(x,y) (((x) >= (y)) ? (x) : (y))

void update_bk(Vector *bk, int mu, Vector *bj, int dim) {
    for (int i = 0; i < dim; i++) {
        bk->e[i] -= mu * bj->e[i];
    }
}


void LLL(Vector2D *B) {
    float delta = 0.75;

    GS_Info *gs_info = gram_schmidt(B);

    int k = 1;
    
    while (k < B->dim) {
        for (int j = k - 1; j >= 0; j--) {
            double rounded_mu = round(gs_info->mu->v[k]->e[j]);
            if (rounded_mu == 0.0) {
                continue;
            }
            // if (rounded_mu > 1.0 || rounded_mu < -1.0) {
            //     printf("Basis: \n");
            //     printVector2D(B);
            //     printf("k: %d\n", k);
            //     printf("j: %d\nB[k]: ", j);
            //     printVector(B->v[k], B->dim);
            //     printf("Rounded mu: %.4f\nB[j]: ", rounded_mu);
            //     printVector(B->v[j], B->dim);
            // }
            if (fabs(gs_info->mu->v[k]->e[j]) > 0.5) {
                update_bk(B->v[k], rounded_mu, B->v[j], B->dim);
            }
            // if (rounded_mu > 1.0 || rounded_mu < -1.0) {
            //     printf("Before\n");
            //     printVector2D(gs_info->mu);
            // }

            gs_info->mu->v[k]->e[j] -= rounded_mu;
            for (int i = j - 1; i >= 0; i--) {
                gs_info->mu->v[k]->e[j] -= rounded_mu * gs_info->mu->v[k - 1]->e[j];
            }
            
            // if (rounded_mu > 1.0 || rounded_mu < -1.0) {
            //     printf("After\n");
            //     printVector2D(gs_info->mu);
            // }
        }
        if (inner_product(gs_info->Bs->v[k], gs_info->Bs->v[k], B->dim) > (delta - (gs_info->mu->v[k]->e[k-1]) * (gs_info->mu->v[k]->e[k-1])) * inner_product(gs_info->Bs->v[k-1], gs_info->Bs->v[k-1], B->dim)) {
            k += 1;
        } else {
            Vector *temp = B->v[k];
            B->v[k] = B->v[k-1];
            B->v[k-1] = temp;

            freeGSInfo(gs_info);
            gs_info = gram_schmidt(B);

            k = max_int(k-1, 1);
        }
    }

    freeGSInfo(gs_info);
}