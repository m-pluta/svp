#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "test.h"

char *readLine(FILE *file) {
    char *line = NULL;
    int len = 0;

    int c;
    // Read next character from file
    // Check if its EOF (end of file) or if its a newline
    while ((c = fgetc(file)) != EOF && c != '\n') {
        // +2 = For the new char and null terminator
        line = realloc(line, len + 2);
        if (!line) {
            return NULL;
        }
        line[len++] = c;
    }

    // Check if the file was just entirely empty
    if (len == 0 && c == EOF) {
        free(line);
        return NULL;
    }

    // Add null terminator to the end of the line
    line[len] = '\0';
    return line;
}

Testcase *getTestcase(char *line) {
    // Malloc memory for the test-case
    Testcase *testcase = malloc(sizeof(Testcase));
    if (testcase == NULL) {
        printf("Unidentified lattice type\n");
    }

    // saveptr to store remainder of comma-separated line
    char *saveptr;
    // Skip first column
    char *token = strtok_r(line, ",", &saveptr);

    // Read in the lattice type and convert to understandable form
    token = strtok_r(NULL, ",", &saveptr);
    if (strcmp(token, "u") == 0) {
        testcase->lattice_type = "UNIFORM";
    } else if (strcmp(token, "r") == 0) {
        testcase->lattice_type = "KNAPSACK";
    } else {
        printf("Unidentified lattice type\n");
        free(testcase);
        return NULL;
    }

    // Extract the dimension and bit level
    token = strtok_r(NULL, ",", &saveptr);
    testcase->dimension = (int)strtol(token, NULL, 10);
    token = strtok_r(NULL, ",", &saveptr);
    testcase->bit_level = (int)strtol(token, NULL, 10);

    // Extract the lattice
    token = strtok_r(NULL, ",", &saveptr);
    testcase->lattice = token;

    // Extract the expected answer
    token = strtok_r(NULL, ",", &saveptr);
    testcase->expected = strtod(token, NULL);

    return testcase;
}

int main() {
    char *TEST_FILE = "tests/test-gen.csv";
    char *RESULT_FILE = "result.txt";
    double TOLERANCE = 0.00005;
    int num_pass = 0;
    int num_fail = 0;

    // Open test file
    FILE *file = fopen(TEST_FILE, "r");
    if (file == NULL) {
        perror("Unable to open file\n");
        return 1;
    }

    // Space to store the human-readable result
    size_t result_size = 5 * sizeof(char);
    char* result = malloc(result_size);

    // Iterate through all the lines in the test-file
    char *line;
    while ((line = readLine(file)) != NULL) {
        // Extract the test case
        Testcase *test_case = getTestcase(line);
        if (test_case == NULL) {
            printf("No test case found\n");
            continue;
        }

        // Define the command
        size_t command_size = strlen(test_case->lattice) + 9;
        char* command = malloc(command_size * sizeof(char));
        snprintf(command, command_size, "./runme %s", test_case->lattice);

        // Run the command
        int return_code = system(command);
        if (return_code == -1) {
            printf("Child process could not be started\n");
            free(line);
            free(command);
        }
        free(command);

        // Open the result.txt file
        FILE *result_file = fopen(RESULT_FILE, "r");
        if (result_file == NULL) {
            perror("Unable to open file\n");
            free(line);
            return 1;
        }

        // Read the result and close the file
        char *actual_result = readLine(result_file);
        fclose(result_file);

        // Calculate the absolute difference and declare space for test result
        double abs_diff = test_case->expected - strtod(actual_result, NULL);

        // Check whether actual answer is within the tolerance of the expected
        if (fabs(abs_diff) <= TOLERANCE) {
            snprintf(result, result_size, "PASS");
            num_pass++;
        } else {
            snprintf(result, result_size, "FAIL");
            printf("%s\n", test_case->lattice);
            num_fail++;
        }

        // Print out test-case information to the command line
        printf("%8s_%2dD_%2db: %s\n", test_case->lattice_type,
               test_case->dimension, test_case->bit_level, result);

        // Free all the memory for the testcase
        free(actual_result);
        free(line);
        free(test_case);
    }

    // Print out test summary
    printf("Tolerance: %f\n", TOLERANCE);
    printf("Passed tests: %d\n", num_pass);
    printf("Failed tests: %d\n", num_fail);
    printf("Accuracy: %.2f%%\n",
            (double)num_pass / (num_pass + num_fail) * 100);

    free(result);
    // Close the file
    fclose(file);
    return 0;
}
