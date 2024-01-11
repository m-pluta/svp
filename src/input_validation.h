#ifndef INPUT_VALIDATION_H
#define INPUT_VALIDATION_H

#include "matrix.h"

int parseInput(Matrix B, const int dim, const int num_args, char *args[]);
int isLinearlyDependent(Matrix Bs, const int dim);

#endif
