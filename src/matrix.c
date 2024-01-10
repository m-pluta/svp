#include <stdio.h>
#include <stdlib.h>

#include "vector.h"
#include "matrix.h"

Matrix mallocMatrix(const int dim)
{
    // Allocate the Matrix Pointer
    Matrix m = malloc(dim * sizeof(Vector));
    if (m == NULL)
    {
        printf("Failed to malloc Matrix\n");
        return NULL;
    }

    // Allocate each vector within that Matrix
    for (int i = 0; i < dim; i++)
    {
        m[i] = mallocVector(dim);
        if (m[i] == NULL)
        {
            // If malloc failed then free all previously allocated vectors
            printf("Failed to malloc Matrix's inner vectors\n");
            for (int j = i - 1; j >= 0; j++)
            {
                freeVector(m[j]);
            }
            free(m);
            m = NULL;
            return NULL;
        }
    }
    return m;
}

void freeMatrix(Matrix m, const int dim)
{
    // Free all vectors in the matrix
    for (int i = 0; i < dim; i++)
    {
        freeVector(m[i]);
    }
    free(m);
    m = NULL;
}

void printMatrix(const Matrix m, const int dim)
{
    printf("Matrix Dimension: %d\n", dim);

    // Iterate through each vector in matrix
    for (int i = 0; i < dim; i++)
    {

        // Print all elements in each vector
        printf("Vector %2.d: [", i + 1);
        for (int j = 0; j < dim; j++)
        {
            // Add spaces between the elements, except after last element
            if (j != 0)
            {
                printf(" ");
            }
            printf("%8.60f", m[i][j]);
        }
        printf("]\n");
    }
}
