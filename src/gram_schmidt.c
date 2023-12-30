#include <stdio.h>
#include <stdlib.h>

#include "vector.h"
#include "vector2d.h"

typedef struct {
    Vector2D *mu;
    Vector2D *Bs;
} Gram_Schmidt_Information;

Gram_Schmidt_Information* gram_schmidt(Vector2D *B) {
    Vector2D *mu = mallocVector2D(B->dim);
    if (mu == NULL) {
        printf("Failed to malloc Vector2D: mu");
        return NULL;
    }
    Vector2D *Bs = mallocVector2D(B->dim);
    if (Bs == NULL) {
        printf("Failed to malloc Vector2D: Bs");
        freeVector2D(mu);
        return NULL;
    }
    Gram_Schmidt_Information* gs_info = malloc(sizeof(Gram_Schmidt_Information));
    if (gs_info == NULL) {
        printf("Failed to malloc Gram_Schmidt_Information: gs_info");
        freeVector2D(mu);
        freeVector2D(Bs);
        return NULL;
    }

    for (int i = 0; i < B->dim; i++) {
        for (int j = 0; j < B->dim; j++) {
            mu->v[i]->e[j] = 0.0;
            Bs->v[i]->e[j] = B->v[i]->e[j];
        }
        for (int k = 0; k < i; k++) {
            mu->v[i]->e[k] = inner_product(B->v[i], Bs->v[k], B->dim) / inner_product(Bs->v[k], Bs->v[k], B->dim);
            // printf("mu[i][k]: %.6f\n", mu->v[i]->e[k]);
            for (int j = 0; j < B->dim; j++) {
                Bs->v[i]->e[j] -= mu->v[i]->e[k] * Bs->v[k]->e[j];
                // printf("Bs[i][j]: %.6f, %.6f, %.6f\n", Bs->v[i]->e[j], mu->v[i]->e[k], Bs->v[k]->e[j]);
            }
        }
    }

    gs_info->mu = mu;
    gs_info->Bs = Bs;

    return gs_info;
}
