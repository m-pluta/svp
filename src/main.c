#include <stdio.h>
#include <string.h>
#include <math.h>

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
        fprintf(file, "%.20f", result);
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
    // Ignore the program name
    argc -= 1;
    argv++;

    // Check number of arguments is at least 1
    if (argc == 0) {
        printf("No basis provided\n");
        printf("Usage: %s [x1 y1 ...] [x2 y2 ...] ...\n", argv[0]);
        return 1;
    }

    // Determine N from the number of arguments passed
    double double_dim = sqrt(argc);
    int dim = round(double_dim);
    if (dim * dim != argc) {
        printf("Invalid Input: Input basis is not square\n");
        return 1;
    }

    // Malloc the Basis
    Matrix B = mallocMatrix(dim);
    if (B == NULL) {
        printf("Failed to malloc Matrix: B");
        return 1;
    }

    // Parse the input
    int ret_code = parseInput(B, dim, argc, argv);
    if (ret_code == 1) {
        // Parsing was unsuccessful
        freeMatrix(B, dim);
        return 1;
    }

    // Malloc the GS_Info struct and perform Gram Schmidt Procedure
    GS_Info *gs_info = mallocGS_Info(dim);
    gram_schmidt(B, gs_info, dim);

    // Check if Basis is linearly independent
    if (isLinearlyDependent(gs_info->Bs, dim)) {
        freeMatrix(B, dim);
        freeGSInfo(gs_info, dim);
        printf("Invalid Input: The input vectors are linearly dependent\n");
        return 1;
    }

    // Perform LLL to get the LLL reduced basis
    // Perform a subsequent GS to update
    LLL(B, gs_info, 0.99, dim);
    gram_schmidt(B, gs_info, dim);

    // Calculate the Minkowski Bound
    double bound = lambda_1_squared(gs_info->Bs, dim);
    // printf("Bound: %.6f\n", bound);

    // Perform Schnorr-Euchner enumeration
    double result = schorr_euchner(dim, gs_info, bound);
    // printf("%8.8f\n", result);

    writeResultToFile(result);

    // Free allocated memory
    freeGSInfo(gs_info, dim);
    freeMatrix(B, dim);
    return 0;
}
