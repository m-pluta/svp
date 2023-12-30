#ifndef LLL_H
#define LLL_H

#include "vector.h"
#include "vector2d.h"

void update_bk(Vector *bk, int mu, Vector *bj, int dim);
void LLL(Vector2D *B);

#endif // LLL_H