#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    double *values;
} Vector;

void freeVector(Vector *vector) {
    free(vector->values);
    vector->values = NULL;
    free(vector);
    vector = NULL;
}

void printVectors(Vector *vectors, int count) {
    for (int i = 0; i < count; i++) {
        printf("Vector %d: [", i + 1);
        for (int j = 0; j < count; j++) {
            printf("%.2lf", vectors[i].values[j]);
            if (j < count - 1) {
                printf(" ");
            }
        }
        printf("]\n");
    }
}

int main(int argc, char *argv[]) {
    // Check number of arguments
    if (argc == 1) {
        printf("Usage: %s [x1 y1 ...] [x2 y2 ...] ...\n", argv[0]);
        return 1;
    }

    // Calculate size of first input vector
    int N = 1;
    for (int i = 1; i < argc; i++) {
        if (argv[i][strlen(argv[i]) - 1] == ']') {
            break;
        }
        N++;
    }
    printf("N: %d\n", N);


    // Parse all arguments
    Vector *basis = malloc(N * sizeof(Vector));
    if (basis == NULL) {
        return 1;
    }


    // // Instantiate a square matrix 3x3 that is the identity matrix
    // for (int i = 0; i < N; i++) {
    //     basis[i].values = malloc((N + 1) * sizeof(double));
    //     if (basis[i].values == NULL) {
    //         // Handle memory allocation error
    //         return 1;
    //     }
    //     basis[i].values[0] = N;
    //     for (int j = 1; j <= N; j++) {
    //         if (i == j - 1) {
    //             basis[i].values[j] = 1.0;
    //         } else {
    //             basis[i].values[j] = 0.0;
    //         }
    //     }
    // }
    // printVectors(basis, N);

    int curr_vector = 0;
    int curr_element = 0;

    for (int i = 1; i < argc; i++) {
        char* curr_arg = argv[i];

        if (curr_arg[0] == '[') {
            basis[curr_vector].values = malloc(N * sizeof(double));
            if (basis[curr_vector].values == NULL) {
                return 1;
            }
            curr_element = 0;
            basis[curr_vector].values[curr_element++] = strtod(++curr_arg, NULL);
            continue;
        }
        if (curr_arg[strlen(curr_arg) - 1] == ']') {
            basis[curr_vector++].values[curr_element++] = strtod(curr_arg, NULL);
            continue;
        }

        // vector.values[curr_element++] = strtod(curr_arg, NULL);
    }

    printVectors(basis, N);

    return 0;
}