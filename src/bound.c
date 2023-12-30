#include <math.h>

#include "vector.h"
#include "vector2d.h"

double hermite_constant(const int dim) {
    return 1 + dim / 4;
}

double volL(const Vector2D *Bs) {
    double volL = 1.0;
    for (int i = 0; i < Bs->dim; i++) {
        volL *= norm(Bs->v[i], Bs->dim);
    }
    return volL;
}

double lambda_1(const Vector2D *Bs) {
    return sqrt(hermite_constant(Bs->dim)) * pow(volL(Bs), 1./Bs->dim);
}