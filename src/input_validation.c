#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#include "matrix.h"

// Returns 1 if parsing encountered an error
int parseInput(Matrix B, const int dim, const int argc, char *argv[]) {
    int curr_v = 0;
    int curr_e = 0;

    for (int i = 0; i < argc; i++) {
        char* arg = argv[i];

        if (curr_e == 0) {
            if (arg[0] == '[') {
                arg++;
            } else {
                goto invalid_input;
            }
        }

        char *pEnd;
        double res = strtod(arg, &pEnd);

        if (pEnd == arg) {
            goto invalid_input;
        }

        B[curr_v][curr_e] = res;

        if (curr_e == dim - 1) {
            if (pEnd[0] == ']') {
                pEnd++;
                curr_v++;
                curr_e = 0;
            } else {
                goto invalid_input;
            }
        } else {
            curr_e++;
        }

        if (pEnd[0] != '\0') {
            goto invalid_input;
        }
    }

    // If user entered [0] or [0.0] etc then this is handled
    if (dim == 1 && B[0][0] == 0) {
        goto invalid_input;
    }

    return 0;

    invalid_input:
    printf("Invalid Input\n");
    return 1;
}

// Returns 1 if linearly dependent
int isLinearlyDependent(Matrix Bs, const int dim) {
    // Values may not be exactly zero
    double TOLERANCE = (double)1 / 100000000000;
    for (int i = dim - 1; i >= 0; i--) {
        // dependent = 1 if current vector contains all zeros
        Vector v = Bs[i];
        int dependent = 1;
        for (int j = 0; j < dim; j++) {
            if (fabs(v[j]) > TOLERANCE) {
                dependent = 0;
                break;
            }
        }
        if (dependent) {
            return 1;
        }
    }
    return 0;
}
