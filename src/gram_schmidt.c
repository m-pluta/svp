#include <stdio.h>
#include <stdlib.h>

#include "vector.h"
#include "vector2d.h"

#include "gram_schmidt.h"

void freeGSInfo(GS_Info *gs_info, const int dim) {
    freeVector2D(gs_info->mu, dim);
    freeVector2D(gs_info->Bs, dim);
    free(gs_info);
}

GS_Info* gram_schmidt(const Vector2D *B, const int dim) {
    Vector2D *mu = mallocVector2D(dim);
    if (mu == NULL) {
        printf("Failed to malloc Vector2D: mu");
        return NULL;
    }
    Vector2D *Bs = mallocVector2D(dim);
    if (Bs == NULL) {
        printf("Failed to malloc Vector2D: Bs");
        freeVector2D(mu, dim);
        return NULL;
    }
    GS_Info* gs_info = malloc(sizeof(GS_Info));
    if (gs_info == NULL) {
        printf("Failed to malloc Gram_Schmidt_Information: gs_info");
        freeVector2D(mu, dim);
        freeVector2D(Bs, dim);
        return NULL;
    }

    double inner_products[dim];

    for (int i = 0; i < dim; i++) {
        for (int j = 0; j < dim; j++) {
            Bs->v[i]->e[j] = B->v[i]->e[j];
        }
        
        for (int k = 0; k < i; k++) {
            mu->v[i]->e[k] = inner_product(B->v[i], Bs->v[k], dim) / inner_products[k];
            // printf("mu[i][k]: %.6f\n", mu->v[i]->e[k]);
            for (int j = 0; j < dim; j++) {
                Bs->v[i]->e[j] -= mu->v[i]->e[k] * Bs->v[k]->e[j];
                // printf("Bs[i][j]: %.6f, %.6f, %.6f\n", Bs->v[i]->e[j], mu->v[i]->e[k], Bs->v[k]->e[j]);
            }
        }

        inner_products[i] = inner_product(Bs->v[i], Bs->v[i], dim);
    }

    gs_info->mu = mu;
    gs_info->Bs = Bs;

    return gs_info;
}
