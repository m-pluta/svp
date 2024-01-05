#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "vector2d.h"
#include "bound.h"
#include "gram_schmidt.h"
#include "lll.h"
#include "schorr_euchner.h"

int parseInput(Vector2D *B, const int dim, const int num_args, char *args[]) {
    int curr_vector = 0;
    int curr_element = 0;

    if (num_args - 1 != dim * dim) {
        printf("Input Basis Dimension Mismatch");
        return 1;
    }

    for (int i = 1; i < num_args; i++) {
        if (curr_vector >= dim && curr_element != 0) {
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

void writeResultToFile(const double result) {
    FILE *file = fopen("result.txt", "w");
    if (file != NULL) {
        fprintf(file, "%lf", result);
        fclose(file);
    } else {
        perror("Unable to open 'result.txt' for writing");
    }
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
    int res = parseInput(B, N, argc, argv);
    if (res == 1) {
        printf("Failed to parse input basis");
        freeVector2D(B, N);
        return 1;
    }

    GS_Info *gs_info = gram_schmidt(B, N);
    LLL(B, gs_info, 0.99, N);
    gram_schmidt_in_place(B, gs_info, N);

    double bound = lambda_1_squared(gs_info->Bs, N);
    // printf("Bound: %.6f\n", bound);

    gram_schmidt_in_place(B, gs_info, N);

    double result = schorr_euchner(N, gs_info, bound);
    // printf("%8.8f\n", result);

    writeResultToFile(result);

    // Free allocated memory
    freeGSInfo(gs_info, N);
    freeVector2D(B, N);

    return 0;
}
