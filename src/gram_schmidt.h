#ifndef GRAM_SCHMIDT_H
#define GRAM_SCHMIDT_H

#include "matrix.h"

typedef struct {
    Matrix mu;
    Matrix Bs;
} GS_Info;

GS_Info *mallocGS_Info(const int dim);
void freeGSInfo(GS_Info *gs_info, const int dim);
void gram_schmidt(Matrix B, GS_Info *gs_info, const int dim);

#endif
