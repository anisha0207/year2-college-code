#include <iostream>  // For input/output operations
#include <sstream>   // For std::istringstream, string stream processing
#include <cstdio>    // For file operations (fopen, fclose, etc.)
#include <cstdlib>   // For general purpose functions
#include "hash_set.h"  // Custom hash set implementation

//using c instead of .h above due to c++ style

// Maximum number of test cases to process
const int TESTS = 100;
// Maximum length of each test case string
const int TEST_LENGTH = 1000;

// Function to run a single test case
bool runTest(const char* testCase) {
    std::istringstream iss(testCase); //Creates an input string stream from the test case string, allowing easy parsing of space-separated values
    char operation;
    int value;
    HashSet<int>* set = nullptr; //c++11 :- null pointer, Declares a pointer to a HashSet<int> and initializes it to nullptr
    bool testPassed = true;

	//iss: std::istringstream is a class that allows you to treat a string as an input stream

    // Process each operation in the test case
    while (iss >> operation) {
        switch (operation) {
            case 'H': // Create hash set
                iss >> value;
                delete set; // Delete previous set if exists
                set = new HashSet<int>(value, false);
                break;
            case 'I': // Insert item
                iss >> value;
                set->Add(value);
                break;
            case 'R': // Remove item
                iss >> value;
                set->Remove(value);
                break;
            case 'C': // Assert contains
                iss >> value;
                if (!set->Exists(value)) {
                    testPassed = false;
                }
                break;
            case 'D': // Assert doesn't contain
                iss >> value;
                if (set->Exists(value)) {
                    testPassed = false;
                }
                break;
            case 'S': // Assert size
                iss >> value;
                if (set->Size() != value) {
                    testPassed = false;
                }
                break;
            case 'L': // Assert load factor
                iss >> value;
                if (set->LoadFactor() != value) {
                    testPassed = false;
                }
                break;
            case 'X': // End of test
                delete set;
                set = nullptr;
                return testPassed;
            default:
                std::cout << "Unknown operation: " << operation << std::endl;
                testPassed = false;
        }
    }
    delete set; // Clean up if 'X' was not encountered
    return testPassed;
}

int main() {
    // Open the test file
    FILE* file = std::fopen("/tests/tests", "r");
    if (!file) {
        std::cout << "Error opening file" << std::endl;
        return 1;
    }

    // Array to store test cases
    char testCases[TESTS][TEST_LENGTH];
    int testCount = 0;

    // Read test cases from file, skipping comment lines
    while (std::fgets(testCases[testCount], TEST_LENGTH, file) && testCount < TESTS) {
        if (testCases[testCount][0] != '-') { // Skip comment lines
            char* newline = std::strchr(testCases[testCount], '\n');
            if (newline) *newline = '\0';  // Remove newline character
            testCount++;
        }
    }
    std::fclose(file);

    int failedTests = 0;

    // Run all test cases
    for (int i = 0; i < testCount; i++) {
        bool testPassed = runTest(testCases[i]);
        if (testPassed) {
            std::cout << "Test " << i + 1 << " passed" << std::endl;
        } else {
            std::cout << "Test " << i + 1 << " failed" << std::endl;
            failedTests++;
        }
    }

    // Print final results
    if (failedTests == 0) {
        std::cout << "All tests passed" << std::endl;
    } else {
        std::cout << failedTests << " tests failed" << std::endl;
    }

    return failedTests;
}
