#include <math.h>

#include "vector2d.h"

double hermite_constant(const int dim) {
    return 1 + dim / 4;
}

double volL(const Vector2D *Bs, const int dim) {
    double volL = 1.0;
    for (int i = 0; i < dim; i++) {
        volL *= norm(Bs->v[i], dim);
    }
    return volL;
}

double lambda_1(const Vector2D *Bs, const int dim) {
    return sqrt(hermite_constant(dim)) * pow(volL(Bs, dim), 1./dim);
}