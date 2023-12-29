#include <stdio.h>
#include <stdlib.h>
#include <vector.h>
#include <vector2d.h>

typedef struct {
    Vector2D *mu;
    Vector2D *Bs;
} Gram_Schmidt_Information;

Gram_Schmidt_Information* gram_schmidt(Vector2D *basis) {
    Vector2D *mu = mallocVector2D(basis->dimension);
    if (mu == NULL) {
        return NULL;
    }
    Vector2D *Bs = mallocVector2D(basis->dimension);
    if (Bs == NULL) {
        freeVector2D(mu);
        return NULL;
    }

    for (int i = 0; i < basis->dimension; i++) {
        for (int j = 0; j < basis->dimension; j++) {
            mu->vectors[i]->values[j] = 0.0;
            Bs->vectors[i]->values[j] = basis->vectors[i]->values[j];
        }
        for (int k = 0; k < i; k++) {
            mu->vectors[i]->values[k] = inner_product(basis->vectors[i], basis->vectors[k], basis->dimension) / inner_product(basis->vectors[k], basis->vectors[k], basis->dimension);
            for (int j = 0; j < basis->dimension; j++) {
                Bs->vectors[i]->values[j] -= mu->vectors[i]->values[k] * Bs->vectors[k]->values[j];
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
