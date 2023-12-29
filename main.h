#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    double *values;
} Vector;

typedef struct {
    Vector **vectors;
    int dimension;
} Vector2D;

Vector* mallocVector(int dimension);
void freeVector(Vector *vector);
void printVector(const Vector *vector, int dimension);

Vector2D* mallocVector2D(int dimension);
void freeVector2D(Vector2D *basis);
void printVector2D(const Vector2D *basis);

int parseInput(Vector2D *basis, int num_args, char *args[]);
