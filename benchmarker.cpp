#include <iostream>         // For console output
#include <list>             // NEW: For std::list
#include <vector>           // For dynamic arrays
#include <chrono>           // For high-resolution timing
#include <fstream>          // For file output
#include <iomanip>          // For output formatting
#include <algorithm>        // NEW: For std::find
#include <sys/resource.h>   // NEW: For memory usage monitoring (Linux/macOS)
#include "hashset.hpp"      // Custom HashSet header

// NEW: Function to get memory usage in KB (Linux/macOS)
size_t get_memory_usage() {
    #ifdef __unix__
        struct rusage usage;
        getrusage(RUSAGE_SELF, &usage);
        return usage.ru_maxrss;  // Returns KB on Linux, bytes on macOS
    #else
        // Simplified version for other platforms (returns 0)
        return 0;
    #endif
}

// NEW: Benchmark class to compare HashSet and std::list
class HashSetBenchmark {
private:
    // Load factor percentages to test
    const std::vector<unsigned int> load_factors = {20, 70, 120};

    // Element counts for benchmarking
    const std::vector<int> element_counts = {1000, 10000, 100000, 1000000};

    // NEW: Storage for benchmark timings
    std::vector<std::vector<double>> custom_insert_times;
    std::vector<std::vector<double>> stl_insert_times;
    std::vector<std::vector<double>> custom_contains_times;
    std::vector<std::vector<double>> stl_contains_times;
    std::vector<std::vector<double>> custom_remove_times;
    std::vector<std::vector<double>> stl_remove_times;

    // NEW: Storage for memory usage data
    std::vector<std::vector<size_t>> custom_memory_usage;
    std::vector<std::vector<size_t>> stl_memory_usage;

    // NEW: Helper: Convert integers to strings for HashSet insertion
    std::vector<std::string> convertToStrings(const std::vector<int>& elements) {
        std::vector<std::string> str_elements;
        for (int val : elements) {
            str_elements.push_back(std::to_string(val));
        }
        return str_elements;
    }

    // NEW: Measure time to insert elements into the custom HashSet
    double benchmarkInsert(HashSet& set, const std::vector<std::string>& elements) {
        auto start = std::chrono::high_resolution_clock::now();
        for (const std::string& val : elements) {
            set.insert(std::stoi(val));  // NEW: Explicit conversion
        }
        auto end = std::chrono::high_resolution_clock::now();
        return std::chrono::duration<double>(end - start).count();
    }

    // NEW: Measure avg time to check if element exists in the custom HashSet
    double benchmarkContains(HashSet& set, const std::vector<std::string>& elements) {
        double totalTime = 0;
        for (const std::string& val : elements) {
            auto start = std::chrono::high_resolution_clock::now();
            set.contains(std::stoi(val));  // NEW: Explicit conversion
            auto end = std::chrono::high_resolution_clock::now();
            totalTime += std::chrono::duration<double>(end - start).count();
        }
        return totalTime / elements.size();
    }

    // NEW: Measure avg time to remove elements from the custom HashSet
    double benchmarkRemove(HashSet& set, const std::vector<std::string>& elements) {
        double totalTime = 0;
        for (const std::string& val : elements) {
            auto start = std::chrono::high_resolution_clock::now();
            set.remove(std::stoi(val));  // NEW: Explicit conversion
            auto end = std::chrono::high_resolution_clock::now();
            totalTime += std::chrono::duration<double>(end - start).count();
        }
        return totalTime / elements.size();
    }

    // NEW: Measure time to insert elements into std::list
    double benchmarkInsertList(std::list<int>& list, const std::vector<int>& elements) {
        auto start = std::chrono::high_resolution_clock::now();
        for (int val : elements) {
            list.push_back(val);
        }
        auto end = std::chrono::high_resolution_clock::now();
        return std::chrono::duration<double>(end - start).count();
    }

    // NEW: Measure avg time to find elements in std::list
    double benchmarkContainsList(std::list<int>& list, const std::vector<int>& elements) {
        double totalTime = 0;
        for (int val : elements) {
            auto start = std::chrono::high_resolution_clock::now();
            std::find(list.begin(), list.end(), val);
            auto end = std::chrono::high_resolution_clock::now();
            totalTime += std::chrono::duration<double>(end - start).count();
        }
        return totalTime / elements.size();
    }

    // NEW: Measure avg time to remove elements from std::list
    double benchmarkRemoveList(std::list<int>& list, const std::vector<int>& elements) {
        double totalTime = 0;
        for (int val : elements) {
            auto start = std::chrono::high_resolution_clock::now();
            list.remove(val);
            auto end = std::chrono::high_resolution_clock::now();
            totalTime += std::chrono::duration<double>(end - start).count();
        }
        return totalTime / elements.size();
    }

public:
    // NEW: Run all benchmarks and store results
    void runBenchmarks() {
        custom_insert_times.clear();
        stl_insert_times.clear();
        custom_contains_times.clear();
        stl_contains_times.clear();
        custom_remove_times.clear();
        stl_remove_times.clear();

        custom_memory_usage.clear();
        stl_memory_usage.clear();

        for (unsigned int load_factor : load_factors) {
            std::vector<double> custom_insert, stl_insert;
            std::vector<double> custom_contains, stl_contains;
            std::vector<double> custom_remove, stl_remove;
            std::vector<size_t> custom_memory, stl_memory; // Memory measurement

            for (int num_elements : element_counts) {
                std::cout << "Benchmarking with load factor " << load_factor
                          << "% and " << num_elements << " elements...\n";

                // Generate test data
                std::vector<int> elements;
                for (int i = 1; i <= num_elements; ++i) {
                    elements.push_back(i);
                }

                // NEW: Convert int elements to string elements
                std::vector<std::string> stringElements = convertToStrings(elements);

                // Benchmark custom hash set
                HashSet custom_set(num_elements / load_factor);
                custom_set.set_load_threshold(load_factor);

                size_t custom_mem_before = get_memory_usage(); // Before insert
                custom_insert.push_back(benchmarkInsert(custom_set, stringElements));  // Call to insert
                size_t custom_mem_after = get_memory_usage();  // After insert
                custom_memory.push_back(custom_mem_after - custom_mem_before); // Memory used

                custom_contains.push_back(benchmarkContains(custom_set, stringElements));
                custom_remove.push_back(benchmarkRemove(custom_set, stringElements));

                // Benchmark STL list
                std::list<int> stl_list;

                size_t stl_mem_before = get_memory_usage();    // Before insert
                stl_insert.push_back(benchmarkInsertList(stl_list, elements));  // Call to insert
                size_t stl_mem_after = get_memory_usage();     // After insert
                stl_memory.push_back(stl_mem_after - stl_mem_before);   // Memory used

                stl_contains.push_back(benchmarkContainsList(stl_list, elements));
                stl_remove.push_back(benchmarkRemoveList(stl_list, elements));
            }

            // Store results for this load factor
            custom_insert_times.push_back(custom_insert);
            stl_insert_times.push_back(stl_insert);
            custom_contains_times.push_back(custom_contains);
            stl_contains_times.push_back(stl_contains);
            custom_remove_times.push_back(custom_remove);

            custom_memory_usage.push_back(custom_memory); // Memory for custom
            stl_memory_usage.push_back(stl_memory); // Memory for STL
        }
    }

    // NEW: Write benchmark results to CSV
    void generateCSV() {
        std::ofstream csv("performance_results.csv");
        if (!csv.is_open()) {
            std::cerr << "Error opening CSV file!\n";
            return;
        }

        // CSV Header
        csv << "DataStructure,LoadFactor,Elements,Operation,Time(ms)\n";

        // Write data for each data structure, load factor, element count, and operation
        for (size_t lf = 0; lf < load_factors.size(); ++lf) {
            for (size_t i = 0; i < element_counts.size(); ++i) {
                // Custom HashSet
                csv << "HashSet," << load_factors[lf] << "," << element_counts[i] << ",Insert,"
                    << custom_insert_times[lf][i] * 1000.0 << "\n"; // Convert to ms
                csv << "HashSet," << load_factors[lf] << "," << element_counts[i] << ",Contains,"
                    << custom_contains_times[lf][i] * 1000.0 << "\n"; // Convert to ms
                csv << "HashSet," << load_factors[lf] << "," << element_counts[i] << ",Remove,"
                    << custom_remove_times[lf][i] * 1000.0 << "\n"; // Convert to ms

                // STL unordered_set
                csv << "std::unordered_set," << load_factors[lf] << "," << element_counts[i] << ",Insert,"
                    << stl_insert_times[lf][i] * 1000.0 << "\n"; // Convert to ms
                csv << "std::unordered_set," << load_factors[lf] << "," << element_counts[i] << ",Contains,"
                    << stl_contains_times[lf][i] * 1000.0 << "\n"; // Convert to ms
                csv << "std::unordered_set," << load_factors[lf] << "," << element_counts[i] << ",Remove,"
                    << stl_remove_times[lf][i] * 1000.0 << "\n"; // Convert to ms
            }
        }
        csv.close();
        std::cout << "Benchmark data written to performance_results.csv\n";
    }
};

// Main function to run the benchmarks
int main() {
    HashSetBenchmark benchmark;
    benchmark.runBenchmarks();
    benchmark.printSummary();
    benchmark.generateCSV();
    return 0;
}
