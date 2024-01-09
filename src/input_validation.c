#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "matrix.h"

int checkValidArgument(char* c) {
    while (*c != '\0') {
        if (isdigit(*c) || *c == ']' || *c == '[' || *c == '.' || *c == '+' || *c == '-') {
            c++;
        } else {
            return 1;
        }
    }
    return 0;
}

int parseInput(Matrix B, const int dim, const int num_args, char *args[]) {
    if (num_args - 1 != dim * dim) {
        printf("Invalid Input: Wrong amount of arguments for square basis\n");
        return 1;
    }

    if (args[1][0] != '[') {
        printf("Invalid Input: No opening bracket for first vector\n");
        return 1;
    }

    
    int curr_v = 0; // Current vector being parsed
    int curr_e = 0;

    for (int i = 1; i < num_args; i++) {
        
        // Skip first character if its an opening bracket
        char* curr_arg = args[i];
        if (curr_e == 0 && curr_arg[0] == '[') {
            curr_arg++;
        }

        // Get length of argument
        int arg_length = strlen(curr_arg);

        // Check if argument contains illegal characters
        if (checkValidArgument(curr_arg) == 1) {
            printf("Invalid Input: Vector %d, element %d contains illegal characters\n", curr_v + 1, curr_e + 1);
            return 1;
        }

        // Convert argument to double
        B[curr_v][curr_e] = strtod(curr_arg, NULL);

        int closing_bracket = curr_arg[arg_length - 1] == ']';
        // Check if closing bracket hasn't appeared
        if (curr_e == dim - 1 && !closing_bracket) {
            printf("Invalid Input: Vector %d is not the correct dimension, expected size: %d\n", curr_v + 1, dim);
            return 1;
        }
        // Check if closing bracket appeared too early
        if (curr_e < dim - 1 && closing_bracket) {
            printf("Invalid Input: Vector %d is not the correct dimension, expected size: %d\n", curr_v + 1, dim);
            return 1;
        }

        // Move to next position to parse into
        curr_e++;
        if (curr_e == dim) {
            curr_v++;
            curr_e = 0;
        }
    }

    return 0;
}