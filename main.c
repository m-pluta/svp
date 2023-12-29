#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    double *values;
} Vector;

Vector* mallocVector(int dimension) {
    Vector* vector = (Vector*)malloc(sizeof(Vector));
    if (vector == NULL) {
        return NULL;
    }
    vector->values = (double*)malloc(dimension * sizeof(double));
    if (vector->values == NULL) {
        free(vector);
        return NULL;
    }
    return vector;
}

void freeVector(Vector *vector) {
    free(vector->values);
    free(vector);
}

void printVector(const Vector *vector, int dimension) {
    printf("Vector: ");
    for (int i = 0; i < dimension; ++i) {
        printf("%.2f", vector->values[i]);
        if (i != dimension - 1) {
            printf(" ");
        }
    }
    printf("\n");
}


typedef struct {
    Vector **vectors;
    int dimension;
} Basis;

Basis* mallocBasis(int dimension) {
    Basis* basis = (Basis*)malloc(sizeof(Basis));
    if (basis == NULL) {
        return NULL;
    }

    basis->vectors = (Vector**)malloc(dimension * sizeof(Vector*));
    if (basis->vectors == NULL) {
        free(basis);
        return NULL;
    }

    for (int i = 0; i < dimension; ++i) {
        basis->vectors[i] = mallocVector(dimension);
        if (basis->vectors[i] == NULL) {
            return NULL;
        }
    }

    basis->dimension = dimension;
    return basis;
}

void freeBasis(Basis *basis) {
    int i;

    for (i = 0; i < basis->dimension; ++i) {
        free(basis->vectors[i]->values);
        free(basis->vectors[i]);
    }

    free(basis->vectors);
}

void printBasis(const Basis *basis) {
    int i, j;

    printf("Basis Dimension: %d\n", basis->dimension);
    
    for (i = 0; i < basis->dimension; ++i) {
        printf("Vector %d: ", i + 1);
        for (j = 0; j < basis->dimension; ++j) {
            printf("%.2f ", basis->vectors[i]->values[j]);
        }
        printf("\n");
    }
}

int parseInput(Basis *basis, int num_args, char *args[]) {
    int curr_vector = 0;
    int curr_element = 0;

    if (num_args - 1 != basis->dimension * basis->dimension) {
        return 1;
    }

    for (int i = 1; i < num_args; i++) {
        if (curr_vector >= basis->dimension && curr_element != 0) {
            return 1;
        }
        
        char* curr_arg = args[i];
        printf("Input : %s\n", curr_arg);

        if (curr_arg[0] == '[') {
            curr_arg++;
        }

        basis->vectors[curr_vector]->values[curr_element++] = strtod(curr_arg, NULL);

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

    // Malloc the basis
    Basis *B = mallocBasis(N);
    if (B == NULL) {
        return 1;
    }

    // Parse the input
    int res = parseInput(B, argc, argv);
    if (res == 1) {
        freeBasis(B);
        return 1;
    }

    // Print final basis and free allocated memory
    printBasis(B);
    freeBasis(B);
    free(B);

    return 0;
}