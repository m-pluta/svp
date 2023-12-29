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
        return NULL;
    }
    Vector2D *Bs = mallocVector2D(B->dim);
    if (Bs == NULL) {
        freeVector2D(mu);
        return NULL;
    }

    for (int i = 0; i < B->dim; i++) {
        for (int j = 0; j < B->dim; j++) {
            mu->v[i]->e[j] = 0.0;
            Bs->v[i]->e[j] = B->v[i]->e[j];
        }
        for (int k = 0; k < i; k++) {
            mu->v[i]->e[k] = inner_product(B->v[i], B->v[k], B->dim) / inner_product(B->v[k], B->v[k], B->dim);
            for (int j = 0; j < B->dim; j++) {
                Bs->v[i]->e[j] -= mu->v[i]->e[k] * Bs->v[k]->e[j];
            }
        }
    }

    Gram_Schmidt_Information* gs_info = malloc(sizeof(Gram_Schmidt_Information));
    if (gs_info == NULL) {
        freeVector2D(mu);
        freeVector2D(Bs);
        return NULL;
    }
    gs_info->mu = mu;
    gs_info->Bs = Bs;

    return gs_info;
}
