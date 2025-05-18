#include "hashset.hpp"
#include <iostream>  // For debugging
#include <chrono>    // For timing measurements

// Constructor: Initialize an empty hash set with a given number of buckets
HashSet::HashSet(size_t initial_size)
    : bucket_count(initial_size), element_count(0), load_threshold(90), load_factor(0) {  // Optimized load threshold
    array = new Node*[bucket_count]();
    // std::cout << "HashSet initialized with " << bucket_count << " buckets.\n";  // Debugging statement
}

// Destructor: Free all allocated memory
HashSet::~HashSet() {
    clear();
    delete[] array;
    // std::cout << "HashSet destroyed.\n";  // Debugging statement
}

// Generate a prehash for an integer item (optimized hash function)
unsigned long HashSet::prehash(int item) const {
    return static_cast<unsigned long>((item * 2654435761) % bucket_count);  // Knuth's multiplicative hash
}

// Convert prehash value into a valid bucket index
unsigned long HashSet::hash(unsigned long prehash) const {
    return prehash % bucket_count;
}

// Insert an integer into the set. Returns true if inserted, false if already present.
bool HashSet::insert(int item) {
    auto start = std::chrono::high_resolution_clock::now();  // Start timing

    unsigned long hash_value = hash(prehash(item));  // Get bucket index
    Node* current = array[hash_value];

    while (current != nullptr) {
        if (current->data == item) {  // Item already exists in the set
            // std::cout << "Item " << item << " already exists.\n";  // Debugging statement
            return false;
        }
        current = current->next;
    }

    Node* new_node = new Node(item);
    new_node->next = array[hash_value];
    array[hash_value] = new_node;

    ++element_count;  // Increment element count
    updateLoadFactor();

    if (load_factor > load_threshold) {  // Rehash if necessary
        rehash(bucket_count * 2);
    }

    auto end = std::chrono::high_resolution_clock::now();  // End timing
    // std::cout << "Inserted item " << item << " in "
    //           << std::chrono::duration<double, std::milli>(end - start).count()
    //           << " ms.\n";  // Debugging statement

    return true;
}

// Remove an integer from the set. Returns true if removed, false if not found.
bool HashSet::remove(int item) {
    auto start = std::chrono::high_resolution_clock::now();  // Start timing

    unsigned long hash_value = hash(prehash(item));  // Get bucket index
    Node* current = array[hash_value];
    Node* prev = nullptr;

    while (current != nullptr) {
        if (current->data == item) {  // Item found, remove it
            if (prev == nullptr) {
                array[hash_value] = current->next;
            } else {
                prev->next = current->next;
            }
            delete current;  // Free memory for the removed node

            --element_count;  // Decrement element count and update load factor
            updateLoadFactor();

            auto end = std::chrono::high_resolution_clock::now();  // End timing
            // std::cout << "Removed item " << item << " in "
            //           << std::chrono::duration<double, std::milli>(end - start).count()
            //           << " ms.\n";  // Debugging statement

            return true;
        }
        prev = current;
        current = current->next;
    }

    auto end = std::chrono::high_resolution_clock::now();  // End timing
    // std::cout << "Item " << item << " not found for removal. Took "
    //           << std::chrono::duration<double, std::milli>(end - start).count()
    //           << " ms.\n";  // Debugging statement

    return false;  // Item not found in the set
}

// Check if an integer exists in the set. Returns true if found, false otherwise.
bool HashSet::contains(int item) const {
    auto start = std::chrono::high_resolution_clock::now();  // Start timing

    unsigned long hash_value = hash(prehash(item));  // Get bucket index
    Node* current = array[hash_value];

    while (current != nullptr) {
        if (current->data == item) {  // Item found in the set
            auto end = std::chrono::high_resolution_clock::now();  // End timing
            // std::cout << "Item " << item << " found in "
            //           << std::chrono::duration<double, std::milli>(end - start).count()
            //           << " ms.\n";  // Debugging statement

            return true;
        }
        current = current->next;
    }

    auto end = std::chrono::high_resolution_clock::now();  // End timing
    // std::cout << "Item " << item << " not found. Took "
    //           << std::chrono::duration<double, std::milli>(end - start).count()
    //           << " ms.\n";  // Debugging statement

    return false;  // Item not found in the set
}

// Return the number of elements currently in the hash set
size_t HashSet::count() const {
    return element_count;
}

// Return the current load factor as a percentage
unsigned int HashSet::load() const {
    return load_factor;
}

// Set a new load factor threshold for resizing
void HashSet::set_load_threshold(unsigned int threshold) {
    load_threshold = threshold;
}

// Remove all elements from the hash set and free allocated memory
void HashSet::clear() {
    for (size_t i = 0; i < bucket_count; ++i) {
        Node* current = array[i];
        while (current != nullptr) {
            Node* temp = current;
            current = current->next;
            delete temp;  // Free memory for each node in this bucket
        }
        array[i] = nullptr;  // Reset bucket to nullptr after clearing it
    }

    element_count = 0;      // Reset element count to zero after clearing all elements
    updateLoadFactor();     // Update load factor after clearing all elements

    // std::cout << "Cleared all elements from HashSet.\n";  // Debugging statement
}

// Update the load factor based on element count and bucket count
void HashSet::updateLoadFactor() {
    load_factor =
        static_cast<unsigned int>((static_cast<double>(element_count) / bucket_count) * 100);
}

// Resize and rehash all elements into a new array with larger size
void HashSet::rehash(size_t new_size) {
    Node** old_array = array;       // Save pointer to old array of buckets
    size_t old_bucket_count = bucket_count;

    array = new Node*[new_size]();  // Allocate memory for new array of buckets and initialize to nullptr

    bucket_count = new_size;        // Update bucket count to new size

    element_count = 0;              // Reset element count (will be updated during re-insertion)

    for (size_t i = 0; i < old_bucket_count; ++i) {
        Node* current = old_array[i];
        while (current != nullptr) {
            insert(current->data);  // Reinsert each element into new array

            Node* temp = current;
            current = current->next;
            delete temp;            // Free memory for old node after reinsertion
        }
        old_array[i] = nullptr;
    }

    delete[] old_array;             // Free memory for old array of buckets after rehashing is complete

    std::cout << "Rehashed from " << old_bucket_count
              << " buckets to " << bucket_count
              << ".\n";   // Debugging statement to track rehashing.
}
