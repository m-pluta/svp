#ifndef VECTOR2D_H
#define VECTOR2D_H

#include "vector.h"

typedef struct {
    Vector **v;
    int dim;
} Vector2D;

Vector2D* mallocVector2D(int dim);
void freeVector2D(Vector2D *v2d);
void printVector2D(Vector2D *v2d);
    
#endif // VECTOR2D_H