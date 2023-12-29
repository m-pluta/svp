#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "vector2d.h"
#include "schorr_euchner.h"

int parseInput(Vector2D *B, int num_args, char *args[]) {
    int curr_vector = 0;
    int curr_element = 0;

    if (num_args - 1 != B->dim * B->dim) {
        printf("Input Basis Dimension Mismatch");
        return 1;
    }

    for (int i = 1; i < num_args; i++) {
        if (curr_vector >= B->dim && curr_element != 0) {
            printf("Input Basis Dimension Mismatch");
            return 1;
        }
        
        char* curr_arg = args[i];
        // printf("Input : %s\n", curr_arg);

        if (curr_arg[0] == '[') {
            curr_arg++;
        }

        B->v[curr_vector]->e[curr_element++] = strtod(curr_arg, NULL);

        if (curr_arg[strlen(curr_arg) - 1] == ']') {
            curr_vector++;
            curr_element = 0;
        }
    }

    return 0;   
}

int main(int argc, char *argv[]) {
    // Check number of arguments
    if (argc == 1) {
        printf("No basis provided");
        printf("Usage: %s [x1 y1 ...] [x2 y2 ...] ...\n", argv[0]);
        return 1;
    }

    // Calculate size of first input vector
    int N;
    for (N = 1; N < argc; N++) {
        if (argv[N][strlen(argv[N]) - 1] == ']') {
            break;
        }
    }
    // printf("N: %d\n", N);

    // Malloc the basis
    Vector2D *B = mallocVector2D(N);
    if (B == NULL) {
        printf("Failed to malloc Vector2D: B");
        return 1;
    }

    // Parse the input
    int res = parseInput(B, argc, argv);
    if (res == 1) {
        printf("Failed to parse input basis");
        freeVector2D(B);
        free(B);
        return 1;
    }

    // Print final basis
    printVector2D(B);

    double result = schorr_euchner(B, 100000000000);
    printf("%.20f\n", result);

    // Gram_Schmidt_Information *gs_info = gram_schmidt(B);
    // if (gs_info == NULL) {
    //     return 1;
    // }
    
    // printf("mu matrix");
    // printVector2D(gs_info->mu);
    // printf("Bs matrix");
    // printVector2D(gs_info->Bs);

    // Free allocated memory
    freeVector2D(B);
    free(B);

    return 0;
}