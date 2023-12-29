#ifndef VECTOR2D_H
#define VECTOR2D_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "vector.h"

typedef struct {
    Vector **vectors;
    int dimension;
} Vector2D;

Vector2D* mallocVector2D(int dimension);
void freeVector2D(Vector2D *basis);
void printVector2D(const Vector2D *basis);

#endif // VECTOR2D_H