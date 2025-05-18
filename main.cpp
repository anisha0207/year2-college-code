#include "hashset.hpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <chrono> // For timing operations

/**
 * @param map The HashMap instance to test.
 * @param numItems The number of items to insert, retrieve, and remove.
 * Adjusted the code under testPerformance() to get a more accurate time in microseconds rather than 
 * milliseconds since the result is show as 0 for many
 */
void testPerformance(HashMap& map, size_t numItems) {
    std::cout << "Testing performance for " << numItems << " items:\n";

    // Measure time for insert
    auto start = std::chrono::high_resolution_clock::now();
    for (size_t i = 0; i < numItems; ++i) {
        map.insert(i, "Value" + std::to_string(i));
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto insertTime = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    std::cout << "Insert time: " << insertTime << " µs\n";

    // Measure time for get
    start = std::chrono::high_resolution_clock::now();
    for (size_t i = 0; i < numItems; ++i) {
        std::string value;
        map.get(i, value);
    }
    end = std::chrono::high_resolution_clock::now();
    auto getTime = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    std::cout << "Get time: " << getTime << " µs\n";

    // Measure time for remove
    start = std::chrono::high_resolution_clock::now();
    for (size_t i = 0; i < numItems; ++i) {
        map.remove(i);
    }
    end = std::chrono::high_resolution_clock::now();
    auto removeTime = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    std::cout << "Remove time: " << removeTime << " µs\n";

    std::cout << "\n";
}

/**
 * Main function to execute commands from a file and test HashMap performance.
 */
int main() {
    HashMap map; // Create a HashMap instance
    #include <iostream>

    std::cout << "Program started!" << std::endl;

    // Open the test cases file
    std::ifstream testFile("tests/test.txt");
    
    if (!testFile.is_open()) { // Check if the file opened successfully
        std::cerr << "Error opening test.txt file.\n";
        return 1;
    }

    // Read commands from the file and execute them
    std::string command;
    
    while (std::getline(testFile, command)) { // Read each line from the file
        std::istringstream iss(command);      // Parse the command using a string stream
        std::string operation;
        iss >> operation;                     // Extract the operation (e.g., Insert, Retrieve)

        if (operation == "Insert") {
            int key;
            std::string value;
            iss >> key >> value;              // Read key and value from the command
            map.insert(key, value);           // Insert into the hash map
            std::cout << "Inserted key " << key << " with value \"" << value << "\".\n";
        } else if (operation == "Retrieve") {
            int key;
            iss >> key;                       // Read key from the command
            std::string value;
            if (map.get(key, value)) {        // Attempt to retrieve the key
                std::cout << "Retrieved value for key " << key << ": \"" << value << "\".\n";
            } else {
                std::cout << "Key " << key << " not found.\n";
            }
        } else if (operation == "Remove") {
            int key;
            iss >> key;                       // Read key from the command
            if (map.remove(key)) {            // Attempt to remove the key
                std::cout << "Removed key " << key << ".\n";
            } else {
                std::cout << "Failed to remove key " << key << ". Key not found.\n";
            }
        } else if (operation == "InsertMultiple") {
            int numKeys;
            iss >> numKeys;                   // Read number of keys to insert
            for (int i = 0; i < numKeys; ++i) {
                map.insert(i, "Value" + std::to_string(i)); // Insert multiple keys with sequential values
            }
        } else if (operation == "RetrieveMultiple") {
            int numKeys;
            iss >> numKeys;                   // Read number of keys to retrieve
            for (int i = 0; i < numKeys; ++i) {
                std::string value;
                map.get(i, value);            // Attempt to retrieve each key silently
            }
        } else if (operation == "RemoveMultiple") {
            int numKeys;
            iss >> numKeys;                   // Read number of keys to remove
            for (int i = 0; i < numKeys; ++i) {
                map.remove(i);                // Remove each key silently
            }
        } else {
            std::cerr << "Unknown command: \"" << operation << "\".\n"; // Handle invalid commands
        }
    }

    testFile.close(); // Close the file after processing all commands

    // Performance testing with different numbers of items
    HashMap performanceMap;                   // Create a new hash map instance for performance testing
    
    testPerformance(performanceMap, 100);     // Test with 100 items
    testPerformance(performanceMap, 1000);    // Test with 1,000 items
    testPerformance(performanceMap, 10000);   // Test with 10,000 items

    return 0;
}
