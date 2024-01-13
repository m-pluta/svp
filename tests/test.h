#ifndef TEST_H
#define TEST_H

// Struct to store a test-case
typedef struct {
    char *lattice_type;
    int dimension;
    int bit_level;
    char *lattice;
    double expected;
} Testcase;

#endif
