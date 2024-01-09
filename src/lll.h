#ifndef LLL_H
#define LLL_H

#include "matrix.h"
#include "gram_schmidt.h"

void LLL(Matrix B, GS_Info *gs_info, const double delta, const int dim);

#endif // LLL_H
