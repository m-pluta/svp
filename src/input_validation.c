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

        // Assert the opening bracket at the start of each vector
        if (curr_e == 0) {
            if (arg[0] == '[') {
                arg++;
            } else {
                printf("No opening bracket in argument %d\n", i + 1);
                return 1;
            }
        }

        // Convert argument to double
        char *pEnd;
        double res = strtod(arg, &pEnd);

        // Check if no conversion occurred
        if (pEnd == arg) {
            printf("Invalid number in argument %d\n", i + 1);
            return 1;
        }

        // Store converted double in the matrix
        B[curr_v][curr_e] = res;

        // Check if its the final element of a vector being converted
        if (curr_e == dim - 1) {
            // Assert the closing bracket at the end of each vector
            if (pEnd[0] == ']') {
                pEnd++;
                curr_v++;
                curr_e = 0;
            } else {
                printf("No closing bracket in argument %d\n", i + 1);
                return 1;
            }
        } else {
            curr_e++;
        }

        // Make sure there are no extra trailing characters
        if (pEnd[0] != '\0') {
            printf("Extra trailing characters in argument %d\n", i + 1);
            return 1;
        }
    }

    // If user entered [0] or [0.0] then this is an invalid basis
    if (dim == 1 && B[0][0] == 0) {
        printf("The [0] basis is not a valid basis\n");
        return 1;
    }

    return 0;
}

// Returns 1 if linearly dependent
int isLinearlyDependent(Matrix Bs, const int dim) {
    // Values may not be exactly zero
    double TOLERANCE = 1e-12;
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

        // Current vector was all zeros
        if (dependent) {
            return 1;
        }
    }
    return 0;
}
