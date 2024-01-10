#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct
{
    char *lattice_type;
    int dimension;
    int bit_level;
    char *lattice;
    double expected;
} Testcase;

char *readLine(FILE *file) {
    char *line = NULL;
    int len = 0;

    int c;
    // Read next character from file
    // Check if its EOF (end of file) or if its a newline
    while ((c = fgetc(file)) != EOF && c != '\n') {
        line = realloc(line, len + 2); // +2 = new char and null terminator
        if (!line) {
            return NULL;
        }
        line[len++] = c;
    }

    // Empty file
    if (len == 0 && c == EOF) {
        free(line);
        return NULL;
    }
    
    // Add null terminator to the end
    line[len] = '\0';
    return line;
}

Testcase* getTestcase(char* line) {
    Testcase *testcase = malloc(sizeof(Testcase));
    if (testcase == NULL) {
        printf("Unidentified lattice type");
    }
    
    // Skip first column
    char *token = strtok(line, ",");
    
    // Read in the lattice type
    token = strtok(NULL, ",");
    if (strcmp(token, "u") == 0) {
        testcase->lattice_type = "UNIFORM";
    } else if (strcmp(token, "r") == 0) {
        testcase->lattice_type = "KNAPSACK";
    } else {
        printf("Unidentified lattice type");
        free(testcase);
        return NULL;
    }

    // Extract the dimension and bit level
    token = strtok(NULL, ",");
    testcase->dimension = (int) strtol(token,NULL, 10);

    token = strtok(NULL, ",");
    testcase->bit_level = (int) strtol(token,NULL, 10);
    
    // Extract the lattice
    token = strtok(NULL, ",");
    testcase->lattice = token;

    // Extract the expected answer
    token = strtok(NULL, ",");
    testcase->expected = strtod(token, NULL);

    return testcase;
}

int main() {
    char* TEST_FILE = "tests/test-gen.csv";
    char* RESULT_FILE = "result.txt";
    double TOLERANCE = 0.00001;
    int num_pass = 0;
    int num_fail = 0;
    
    FILE *file = fopen(TEST_FILE, "r");
    if (file == NULL) {
        perror("Unable to open file");
        return 1;
    }

    char *line;
    while ((line = readLine(file)) != NULL) {
        Testcase *test_case = getTestcase(line);

        
        if (test_case == NULL) {
            printf("No test case found on line\n");
        }

        char command[strlen(test_case->lattice) + 8];
        sprintf(command, "./runme %s", test_case->lattice);
        int return_code = system(command);
        if (return_code == -1) {
            printf("Child process could not be started\n");
        }

        FILE *result_file = fopen(RESULT_FILE, "r");
        if (result_file == NULL) {
            perror("Unable to open file");
            return 1;
        }

        
        char* result;
        double diff = test_case->expected - strtod(readLine(result_file), NULL);
        if (fabs(diff) < TOLERANCE) {
            result = "PASS";
            num_pass++;
        } else {
            result = "FAIL";
            num_fail++;
        }

        printf("%8s_%2dD_%2db: %s\n", test_case->lattice_type, test_case->dimension, test_case->bit_level, result);

        free(line);
    }

    printf("Tolerance: %f\n", TOLERANCE);
    printf("Passed tests: %d\n", num_pass);
    printf("Failed tests: %d\n", num_fail);
    printf("Accuracy: %.2f%%\n",(double) num_pass / (num_pass + num_fail) * 100);

    // Close the file
    fclose(file);
    return 0;

}