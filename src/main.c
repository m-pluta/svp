#include <stdio.h>
#include <string.h>

#include "input_validation.h"
#include "matrix.h"
#include "bound.h"
#include "gram_schmidt.h"
#include "lll.h"
#include "schnorr_euchner.h"

void writeResultToFile(const double result) {
    // Open result file and output shortest norm with 12dp precision
    FILE *file = fopen("result.txt", "w");
    if (file != NULL) {
        fprintf(file, "%.12f", result);
        fclose(file);
    } else {
        perror("Unable to open 'result.txt' for writing");
    }
}

int findFirstClosingBracket(int argc, char *argv[]) {
    for (int i = 1; i < argc; i++) {
        if (argv[i][strlen(argv[i]) - 1] == ']') {
            // Found the argument ending with ']'
            return i;
        }
    }
    // Indicate no closing bracket found
    return -1;
}

int main(int argc, char *argv[]) {
    // Check number of arguments is at least 1
    if (argc == 1) {
        printf("No basis provided");
        printf("Usage: %s [x1 y1 ...] [x2 y2 ...] ...\n", argv[0]);
        return 1;
    }

    // Calculate size of first input vector
    int N = findFirstClosingBracket(argc, argv);
    if (N == -1) {
        printf("Invalid Input: No closing bracket\n");
        return 1;
    }
    // printf("N: %d\n", N);

    // Malloc the Basis
    Matrix B = mallocMatrix(N);
    if (B == NULL) {
        printf("Failed to malloc Matrix: B");
        return 1;
    }

    // Parse the input
    int res = parseInput(B, N, argc, argv);
    if (res == 1) {
        // Parsing was unsuccessful
        freeMatrix(B, N);
        return 1;
    }

    // Malloc the GS_Info struct and perform Gram Schmidt Procedure
    GS_Info *gs_info = mallocGS_Info(N);
    gram_schmidt(B, gs_info, N);

    // Check if Basis is linearly independent
    if (isLinearlyDependent(gs_info->Bs, N)) {
        freeMatrix(B, N);
        freeGSInfo(gs_info, N);
        printf("Invalid Input: The input vectors are linearly dependent\n");
        return 1;
    }

    // Perform LLL to get the LLL reduced basis
    // Perform a subsequent GS to update
    LLL(B, gs_info, 0.75, N);
    gram_schmidt(B, gs_info, N);

    // Calculate the Minkowski Bound
    double bound = lambda_1_squared(gs_info->Bs, N);
    // printf("Bound: %.6f\n", bound);

    // Perform Schnorr-Euchner enumeration
    double result = schorr_euchner(N, gs_info, bound);
    // printf("%8.8f\n", result);

    writeResultToFile(result);

    // Free allocated memory
    freeGSInfo(gs_info, N);
    freeMatrix(B, N);
    return 0;
}
