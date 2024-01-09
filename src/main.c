#include <stdio.h>
#include <string.h>

#include "input_validation.h"
#include "matrix.h"
#include "bound.h"
#include "gram_schmidt.h"
#include "lll.h"
#include "schnorr_euchner.h"


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
        } else if (N == argc - 1) {
            printf("Invalid Input: No closing bracket\n");
            return 1;
        }
    }
    printf("N: %d\n", N);

    // Malloc the basis
    Matrix B = mallocMatrix(N);
    if (B == NULL) {
        printf("Failed to malloc Matrix: B");
        return 1;
    }

    // Parse the input
    int res = parseInput(B, N, argc, argv);
    if (res == 1) {
        freeMatrix(B, N);
        return 1;
    }

    GS_Info *gs_info = mallocGS_Info(N);
    gram_schmidt(B, gs_info, N);
    if (isLinearlyDependent(gs_info->Bs, N)) {
        freeMatrix(B, N);
        freeGSInfo(gs_info, N);
        printf("Invalid Input: The input vectors are not linearly independent\n");
        return 1;
    }
    LLL(B, gs_info, 0.99999999, N);
    gram_schmidt(B, gs_info, N);

    double bound = lambda_1_squared(gs_info->Bs, N);
    // printf("Bound: %.6f\n", bound);

    double result = schorr_euchner(N, gs_info, bound);
    // printf("%8.8f\n", result);

    writeResultToFile(result);

    // Free allocated memory
    freeGSInfo(gs_info, N);
    freeMatrix(B, N);

    return 0;
}
