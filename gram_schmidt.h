#ifndef GRAM_SCHMIDT_H
#define GRAM_SCHMIDT_H

#include <vector2d.h>

typedef struct {
    Vector2D *mu;
    Vector2D *Bs;
} Gram_Schmidt_Information;

Gram_Schmidt_Information* gram_schmidt(Vector2D *B);

#endif
