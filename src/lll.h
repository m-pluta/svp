#ifndef LLL_H
#define LLL_H

#include "vector2d.h"
#include "gram_schmidt.h"

void LLL(Vector2D *B, GS_Info *gs_info, const double delta, const int dim);

#endif // LLL_H