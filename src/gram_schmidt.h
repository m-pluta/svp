#ifndef GRAM_SCHMIDT_H
#define GRAM_SCHMIDT_H

#include "vector2d.h"

typedef struct {
    Vector2D *mu;
    Vector2D *Bs;
} GS_Info;

void freeGSInfo(GS_Info *gs_info, const int dim);
GS_Info* gram_schmidt(const Vector2D *B, const int dim);
void gram_schmidt_in_place(Vector2D *B, GS_Info *gs_info, const int dim);

#endif
