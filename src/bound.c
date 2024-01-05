#include <math.h>

#include "vector2d.h"

double hermite_constant(const int dim) {
    return 1 + (double) dim / 4;
}

double volL(const Vector2D *Bs, const int dim) {
    double volL = 1.0;
    for (int i = 0; i < dim; i++) {
        volL *= norm(Bs->v[i], dim);
    }
    return volL;
}

// Return the un-square-rooted version
// Avoids doing sqrt just to then square again in the SE algo
double lambda_1_squared(const Vector2D *Bs, const int dim) {
    return hermite_constant(dim) * pow(volL(Bs, dim), 2./dim);
}
