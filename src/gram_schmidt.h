#ifndef GRAM_SCHMIDT_H
#define GRAM_SCHMIDT_H

#include "vector2d.h"

typedef struct {
    Vector2D *mu;
    Vector2D *Bs;
} GS_Info;

void freeGSInfo(GS_Info *gs_info);
GS_Info* gram_schmidt(const Vector2D *B);

#endif
