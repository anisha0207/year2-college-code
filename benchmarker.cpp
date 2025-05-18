#include <iostream>
#include <unordered_set>
#include <vector>
#include <chrono>
#include <fstream>
#include <iomanip>
#include <sys/resource.h>
#include "hashset.hpp"

// Function to get memory usage (Linux/macOS)
size_t get_memory_usage() {
    struct rusage usage;
    getrusage(RUSAGE_SELF, &usage);
    return usage.ru_maxrss; // Memory in kilobytes
}

class HashSetBenchmark {
private:
    const std::vector<unsigned int> load_factors = {20, 70, 120};
    const std::vector<int> element_counts = {1000, 10000, 100000, 1000000};

    // Store benchmark data
    std::vector<std::vector<double>> custom_insert_times;
    std::vector<std::vector<double>> stl_insert_times;
    std::vector<std::vector<double>> custom_contains_times;
    std::vector<std::vector<double>> stl_contains_times;
    std::vector<std::vector<double>> custom_remove_times;
    std::vector<std::vector<double>> stl_remove_times;

    // Benchmark functions (same as before - omitted for brevity)
     double benchmarkInsert(HashSet& set, const std::vector<int>& elements) {
        auto start = std::chrono::high_resolution_clock::now();
        for (int val : elements) {
            set.insert(std::to_string(val));
        }
        auto end = std::chrono::high_resolution_clock::now();
        return std::chrono::duration<double>(end - start).count();
    }

    // Measure average time to check if each element exists in the custom HashSet
    double benchmarkContains(HashSet& set, const std::vector<int>& elements) {
        auto start = std::chrono::high_resolution_clock::now();
        for (int val : elements) {
            set.contains(std::to_string(val));
        }
        auto end = std::chrono::high_resolution_clock::now();
        return std::chrono::duration<double>(end - start).count() / elements.size();
    }

    // Measure average time to remove each element from the custom HashSet
    double benchmarkRemove(HashSet& set, const std::vector<int>& elements) {
        auto start = std::chrono::high_resolution_clock::now();
        for (int val : elements) {
            set.remove(std::to_string(val));
        }
        auto end = std::chrono::high_resolution_clock::now();
        return std::chrono::duration<double>(end - start).count() / elements.size();
    }

    // Measure average time to insert elements into STL unordered_set
    double benchmarkInsert(std::unordered_set<int>& set, const std::vector<int>& elements) {
        auto start = std::chrono::high_resolution_clock::now();
        for (int val : elements) {
            set.insert(val);
        }
        auto end = std::chrono::high_resolution_clock::now();
        return std::chrono::duration<double>(end - start).count() / elements.size();
    }

    // Measure average time to find elements in STL unordered_set
    double benchmarkContains(std::unordered_set<int>& set, const std::vector<int>& elements) {
        auto start = std::chrono::high_resolution_clock::now();
        for (int val : elements) {
            set.find(val);
        }
        auto end = std::chrono::high_resolution_clock::now();
        return std::chrono::duration<double>(end - start).count() / elements.size();
    }

    // Measure average time to remove elements from STL unordered_set
    double benchmarkRemove(std::unordered_set<int>& set, const std::vector<int>& elements) {
        auto start = std::chrono::high_resolution_clock::now();
        for (int val : elements) {
            set.erase(val);
        }
        auto end = std::chrono::high_resolution_clock::now();
        return std::chrono::duration<double>(end - start).count() / elements.size();
    }

public:
    // Run all combinations of benchmarks for each load factor and dataset size
    void runBenchmarks() {
        custom_insert_times.clear();
        stl_insert_times.clear();
        custom_contains_times.clear();
        stl_contains_times.clear();
        custom_remove_times.clear();
        stl_remove_times.clear();

        for (unsigned int load_factor : load_factors) {
            std::vector<double> custom_insert, stl_insert;
            std::vector<double> custom_contains, stl_contains;
            std::vector<double> custom_remove, stl_remove;

            for (int num_elements : element_counts) {
                std::vector<int> elements;
                for (int i = 1; i <= num_elements; ++i) {
                    elements.push_back(i);
                }

                // Benchmark custom hash set
                HashSet custom_set(num_elements / (load_factor==0?1:load_factor));
                custom_set.set_load_threshold(load_factor);
                custom_insert.push_back(benchmarkInsert(custom_set, elements));
                custom_contains.push_back(benchmarkContains(custom_set, elements));
                custom_remove.push_back(benchmarkRemove(custom_set, elements));

                // Benchmark STL unordered_set
                std::unordered_set<int> stl_set;
                stl_set.max_load_factor(load_factor / 100.0);
                stl_set.reserve(num_elements);
                stl_insert.push_back(benchmarkInsert(stl_set, elements));
                stl_contains.push_back(benchmarkContains(stl_set, elements));
                stl_remove.push_back(benchmarkRemove(stl_set, elements));
            }

            custom_insert_times.push_back(custom_insert);
            stl_insert_times.push_back(stl_insert);
            custom_contains_times.push_back(custom_contains);
            stl_contains_times.push_back(stl_contains);
            custom_remove_times.push_back(custom_remove);
            stl_remove_times.push_back(stl_remove);
        }
    }

    // Generate CSV file for plotting
    void generateCSV() {
        std::ofstream csv("performance_results.csv");
        if (!csv.is_open()) {
            std::cerr << "Error opening CSV file!\n";
            return;
        }

        // CSV Header
        csv << "DataStructure,LoadFactor,Operation,Elements,TimeMS\n";

        // Write data for each data structure, load factor, element count, and operation
        for (size_t lf = 0; lf < load_factors.size(); ++lf) {
            for (size_t i = 0; i < element_counts.size(); ++i) {
                // Custom HashSet
                csv << "HashSet," << load_factors[lf] << ",Insert," << element_counts[i] << ","
                    << custom_insert_times[lf][i] * 1000.0 << "\n"; // Convert to ms
                csv << "HashSet," << load_factors[lf] << ",Contains," << element_counts[i] << ","
                    << custom_contains_times[lf][i] * 1000.0 << "\n"; // Convert to ms
                csv << "HashSet," << load_factors[lf] << ",Remove," << element_counts[i] << ","
                    << custom_remove_times[lf][i] * 1000.0 << "\n"; // Convert to ms

                // STL unordered_set
                csv << "STL," << load_factors[lf] << ",Insert," << element_counts[i] << ","
                    << stl_insert_times[lf][i] * 1000.0 << "\n"; // Convert to ms
                csv << "STL," << load_factors[lf] << ",Contains," << element_counts[i] << ","
                    << stl_contains_times[lf][i] * 1000.0 << "\n"; // Convert to ms
                csv << "STL," << load_factors[lf] << ",Remove," << element_counts[i] << ","
                    << stl_remove_times[lf][i] * 1000.0 << "\n"; // Convert to ms
            }
        }
        csv.close();
        std::cout << "Benchmark data written to performance_results.csv\n";
    }

    void printSummary() {
         //This part could be left out because charts already shows everything.
    }
};

int main() {
    HashSetBenchmark benchmark;
    benchmark.runBenchmarks();
    benchmark.generateCSV();
    return 0;
}
