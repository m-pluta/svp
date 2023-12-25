#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

void runTest(int n, int expr) {
    int act = _func(n);
    printf("For numbers: %d Expected: %llu Got: %llu\n", n, expr, act);
    if (act != expr)
        printf("Expected %llu, got %llu\n", expr, act);
    assert(act == expr);
}

int main() {
    // New test case added
    runTest(0, 1);
    return 0;
}

