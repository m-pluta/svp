#include <stdlib.h>
#include <stdio.h>

typedef double *Vector;

void printVector(const Vector v, const int dim)
{
    printf("Vector: [");
    for (int i = 0; i < dim; ++i)
    {
        // Add spaces between all elements, except after the last one
        if (i != 0)
        {
            printf(" ");
        }
        printf("%19.16f", v[i]);
    }
    printf("]\n");
}

double inner_product(const Vector v1, const Vector v2, const int dim)
{
    double total = 0;
    for (int i = 0; i < dim; i++)
    {
        total += v1[i] * v2[i];
    }
    return total;
}

int main(int argc, char* argv[]) {
    if (argc != 7) {
        return 1;
    }
    int DIM = 3;

    // Ignore first argument
    argc--;
    argv++;

    // Malloc the vectors 
    Vector A = malloc(DIM * sizeof(double));
    Vector B = malloc(DIM * sizeof(double));
    Vector C = malloc(DIM * sizeof(double));

    // Load the vectors
    // 3763617828.00000000000000 0.00000000000000 1.00000000000000 
    for (int i = 0; i < DIM; i++) {
        double val = strtod(argv[i], NULL);
        A[i] = val;
        B[i] = val;
    }
    // 359069345.00000000000000 1.00000000000000 0.00000000000000
    for (int i = 0; i < DIM; i++) {
        C[i] = strtod(argv[DIM+i], NULL);
    }

    // Print the vectors
    printf("     A: ");
    printVector(A, DIM);
    printf("     B: ");
    printVector(B, DIM);
    printf("     C: ");
    printVector(C, DIM);


    double mu = inner_product(A, C, DIM) / inner_product(C, C, DIM);

    // Calculate B = B - mu * C
    for (int j = 0; j < DIM; j++){
        // printf("B[j] (Before): %.60f\n", B[j]);
        // printf("    mu * C[j]: %.60f\n", mu * C[j]);
        double test = mu * C[j];
        B[j] = B[j] - test;

        //printf(" B[j] (After): %.60f\n\n", B[j]);
    }

    printf("B-mu*C: ");
    printVector(B, DIM);
    
    return 0;
}