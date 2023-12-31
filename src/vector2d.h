#ifndef VECTOR2D_H
#define VECTOR2D_H

#include "vector.h"

typedef struct {
    Vector **v;
} Vector2D;

Vector2D* mallocVector2D(const int dim);
void freeVector2D(Vector2D *v2d, const int dim);
void printVector2D(const Vector2D *v2d, const int dim);
    
#endif // VECTOR2D_H