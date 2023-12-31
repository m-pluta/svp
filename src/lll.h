#ifndef LLL_H
#define LLL_H

#include "vector.h"
#include "vector2d.h"

void update_bk(Vector *B_k, int mu, Vector *B_j, int dim);
void LLL(Vector2D *B);

#endif // LLL_H