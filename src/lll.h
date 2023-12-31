#ifndef LLL_H
#define LLL_H

#include "vector.h"
#include "vector2d.h"

void update_bk(Vector *B_k, const int mu, const Vector *B_j, const int dim);
void LLL(Vector2D *B);

#endif // LLL_H