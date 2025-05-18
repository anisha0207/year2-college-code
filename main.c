#include "linkedlist.h"
#include "testCase.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TEST 30      // Max number of test cases
#define LENGTH 100   // Max characters per test case (including null terminator)

// Function to read tests from a file
// Two parameters: filename (the name of the file to read) and tests (a 2D array to store the test cases)
int read(const char* filename, char tests[TEST][LENGTH]) {
    FILE* file = fopen(filename, "r"); // Open the file in read mode
    if (!file) { // Check if the file was opened successfully
        printf("Error: Can't open %s\n", filename); // Print an error message if the file couldn't be opened
        return -1; // Return -1 to indicate failure
    }

    int count = 0; // Initialize the count of test cases read
    char buffer[LENGTH]; // Temporary buffer to hold each line from the file

    // This loop reads lines from the file until:
    // - The maximum number of test cases (TEST) is reached
    // - Or there are no more lines in the file
    while (count < TEST && fgets(buffer, LENGTH, file)) {
    // Remove newline character if it exists
    buffer[strcspn(buffer, "\n")] = '\0';

    // Skip empty lines, comments, and invalid lines
    if (strlen(buffer) == 0 || buffer[0] == '-' || buffer[0] == '*' || !strchr("HTNDCEAQZSWX", buffer[0])) {
        continue; // Go to the next iteration of the loop
    }

    // Copy valid test cases
    strncpy(tests[count], buffer, LENGTH - 1);
    tests[count][LENGTH - 1] = '\0';
    count++;
}

    fclose(file); // Close the file after reading all lines

    // Debugging Output: Print all loaded tests
    //printf("Loaded %d Test Cases:\n", count);
    // for (int i = 0; i < count; i++) {
    //     printf("Test %d: %s\n", i + 1, tests[i]);
    // }

    return count; // Return the number of test cases successfully read
}

int main() {
    char tests[TEST][LENGTH];//declares 2D array as tests to store the test cases
    const char* testFile = "tests"; // Hardcoded test file name for simplicity
    // Read the tests from the file
    int test_count = read(testFile, tests);//calls the read_tests function, passing the file name and the tests array.
    if (test_count == -1) {
        return 1; // Exit if the test file couldn't be read
    }//if read_tests encountered an error (indicated by returning -1)

    int fails = 0;

    // Loop through each test and execute it
    for (int i = 0; i < test_count; i++) {
        List list;
        initList(&list);//'&' used is important during debugging since it is the "address-of" function that returns the address of list variable

        printf("\nExecuting Test %d: %s\n", i + 1, tests[i]);
        int result = test(&list, tests[i]);
        if (result == 0) {
            printf("Test %d passed\n", i + 1);
        } else {
            printf("Test %d failed\n", i + 1);
            fails++;
        }
        freeList(&list);
    }

    // Print results
    printf("Passed: %d\n", test_count - fails);
    printf("Failed: %d\n", fails);

    return fails; // Return the number of failed tests as the program's exit code
}
