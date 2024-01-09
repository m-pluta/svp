#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#include "matrix.h"

// Returns 1 if argument is valid
int isValidArgument(char* c) {
    while (*c != '\0') {
        if (isdigit(*c) || *c == ']' || *c == '[' || *c == '.' || *c == '+' || *c == '-') {
            c++;
        } else {
            return 0;
        }
    }
    return 1;
}

// Returns 1 if parsing encountered an error
int parseInput(Matrix B, const int dim, int num_args, char *args[]) {
    // Remove ./runme from args
    num_args--;
    args++;

    // Check first arg has an opening bracket
    if (args[0][0] != '[') {
        printf("Invalid Input: No opening bracket for first vector\n");
        return 1;
    }

    // Check num_args == dim * dim
    if (num_args != dim * dim) {
        printf("Invalid Input\n");
        return 1;
    }

    // Keep track of which vector is currently being parsed
    int curr_v = 0;
    int curr_e = 0;

    for (int i = 0; i < num_args; i++) {
        char* curr_arg = args[i];

        if (!isValidArgument(curr_arg)) {
            printf("Argument %d is invalid\n", i + 1);
        }

        // Assert the presence of opening brackets where expected
        if (curr_e == 0 && curr_arg[0] != '[') {
            printf("Missing opening bracket where expected\n");
            return 1;
        }
        
        // Skip first character if its an opening bracket
        if (curr_arg[0] == '[') {
            if (curr_e == 0) {
                curr_arg++;
            } else {
                printf("Invalid Input: Unexpected opening bracket in vector %d, element %d\n", curr_v + 1, curr_e + 1);
                return 1;
            }
        }

        // Convert to double and keep parseEnding
        char* pEnd;
        double parsed_arg = strtod(curr_arg, &pEnd);

        // Process closing bracket
        int closing_bracket = pEnd[0] == ']';
        if (closing_bracket && curr_e == dim - 1) {
            pEnd++;
        }

        // No closing bracket where it was expected
        if (curr_e == dim - 1 && !closing_bracket) {
            printf("Invalid Input: Vector %d is not the correct dimension, expected size: %d\n", curr_v + 1, dim);
            return 1;
        }

        // There is still more to parse, so must be invalid input
        if (pEnd[0] != '\0') {
            printf("Invalid Input\n");
            return 1;
        }

        // Store the parsed value
        B[curr_v][curr_e] = parsed_arg;

        // Move to next position to parse into
        curr_e++;
        if (curr_e == dim) {
            curr_v++;
            curr_e = 0;
        }
    }

    // If user entered [0] or [0.0] etc then this is handled
    if (dim == 1 && B[0][0] == 0) {
        printf("Invalid Input: Not a basis\n");
        return 1;
    }

    return 0;
}

// Returns 1 if linearly dependent
int isLinearlyDependent(Matrix Bs, const int dim) {
    double TOLERANCE = (double) 1 / 100000000000;
    for (int i = dim - 1; i >= 0; i--) {
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