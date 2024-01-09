#include <math.h>

#include "matrix.h"

double hermite_constant(const int dim)
{
    return 1 + (double)dim / 4;
}

// Return volL which is the product of the norms of the basis vectors
double volL(const Matrix Bs, const int dim)
{
    double volL = 1.0;
    for (int i = 0; i < dim; i++)
    {
        volL *= norm(Bs[i], dim);
    }
    return volL;
}

// Return the un-square-rooted version
// Avoids doing sqrt just to then square again in the SE algo
double lambda_1_squared(const Matrix Bs, const int dim)
{
    return hermite_constant(dim) * pow(volL(Bs, dim), 2. / dim);
}
