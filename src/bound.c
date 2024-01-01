#include <math.h>

#include "vector2d.h"

double hermite_constant(const int dim) {
    return 1 + dim / 4;
}

long double inner_productl(const Vector *v1, const Vector *v2, const int dim) {
    long double res = 0;
    for (int i = 0; i < dim; i++) {
        res += v1->e[i] * v2->e[i];
    }
    return res;
}

long double norml(const Vector *v, const int dim) {
    return sqrtl(inner_productl(v, v, dim));
}

long double volL(const Vector2D *Bs, const int dim) {
    long double volL = 1.0;
    for (int i = 0; i < dim; i++) {
        volL *= norml(Bs->v[i], dim);
    }
    return volL;
}

long double lambda_1(const Vector2D *Bs, const int dim) {
    return sqrtl(hermite_constant(dim)) * powl(volL(Bs, dim), 1./dim);
}