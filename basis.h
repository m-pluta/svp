#ifndef BASIS_H
#define BASIS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "vector.h"

typedef struct {
    Vector **vectors;
    int dimension;
} Basis;

Basis* mallocBasis(int dimension);
void freeBasis(Basis *basis);
void printBasis(const Basis *basis);

#endif // BASIS_H