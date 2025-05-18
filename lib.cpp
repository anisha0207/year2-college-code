#include "hashset.hpp"
#include <iostream>

// Constructor
// Initializes the hash set with the given number of buckets.
// Sets default values for element count, load threshold, and load factor.
// Allocates an array of pointers to Node, initialized to nullptr.
HashSet::HashSet(size_t initial_size)
    : bucket_count(initial_size), element_count(0), load_threshold(70), load_factor(0) {
    array = new Node*[bucket_count]();
}

// Destructor
// Frees all memory by clearing the hash set and deleting the array.
HashSet::~HashSet() {
    clear();
    delete[] array;
}

// Prehash
// Uses djb2 hash algorithm to convert a string into a hash value.
// Returns the raw hash value (not yet reduced to a bucket index).
unsigned long HashSet::prehash(const std::string& item) const {
    unsigned long hash = 5381;
    for (char c : item) {
        hash = ((hash << 5) + hash) + c; // hash * 33 + c
    }
    return hash;
}

// Hash
// Converts the prehash value into a valid bucket index by using modulo.
unsigned long HashSet::hash(unsigned long prehash) const {
    return prehash % bucket_count;
}

// Insert
// Adds a new string to the hash set if it doesn't already exist.
// Handles collisions by inserting at the head of the linked list in the bucket.
// Rehashes the table if the load factor exceeds the threshold.
bool HashSet::insert(const std::string& item) {
    if (contains(item)) return false;

    unsigned long index = hash(prehash(item));
    Node* new_node = new Node(item);
    new_node->next = array[index];
    array[index] = new_node;
    element_count++;

    updateLoadFactor();
    if (load_factor > load_threshold) {
        rehash(bucket_count * 2);
    }

    return true;
}

// Remove
// Deletes an item from the hash set if it exists.
// Traverses the linked list at the bucket to find and remove the node.
bool HashSet::remove(const std::string& item) {
    unsigned long index = hash(prehash(item));
    Node* current = array[index];
    Node* prev = nullptr;

    while (current) {
        if (current->data == item) {
            if (prev) {
                prev->next = current->next;
            } else {
                array[index] = current->next;
            }
            delete current;
            element_count--;
            updateLoadFactor();
            return true;
        }
        prev = current;
        current = current->next;
    }
    return false;
}

// Contains
// Checks whether an item exists in the hash set.
// Traverses the linked list in the appropriate bucket.
bool HashSet::contains(const std::string& item) const {
    unsigned long index = hash(prehash(item));
    Node* current = array[index];
    while (current) {
        if (current->data == item) {
            return true;
        }
        current = current->next;
    }
    return false;
}

// Count
// Returns the number of elements currently stored in the hash set.
size_t HashSet::count() const {
    return element_count;
}

// Load
// Returns the current load factor (as an integer percentage).
unsigned int HashSet::load() const {
    return load_factor;
}

// Set load threshold
// Allows user to modify the load factor threshold for triggering rehash.
void HashSet::set_load_threshold(unsigned int threshold) {
    load_threshold = threshold;
}

// Clear
// Removes all elements from the hash set.
// Deletes all linked list nodes and resets all buckets to nullptr.
void HashSet::clear() {
    for (size_t i = 0; i < bucket_count; ++i) {
        Node* current = array[i];
        while (current) {
            Node* temp = current;
            current = current->next;
            delete temp;
        }
        array[i] = nullptr;
    }
    element_count = 0;
    updateLoadFactor();
}

// Update load factor
// Recalculates the load factor based on element count and bucket count.
void HashSet::updateLoadFactor() {
    load_factor = static_cast<unsigned int>((element_count * 100) / bucket_count);
}

// Rehash
// Doubles the number of buckets and re-inserts all existing elements into new buckets.
// Frees the memory of the old bucket array and its nodes.
void HashSet::rehash(size_t new_size) {
    Node** old_array = array;
    size_t old_bucket_count = bucket_count;

    bucket_count = new_size;
    array = new Node*[bucket_count]();
    element_count = 0;

    for (size_t i = 0; i < old_bucket_count; ++i) {
        Node* current = old_array[i];
        while (current) {
            insert(current->data);
            Node* temp = current;
            current = current->next;
            delete temp;
        }
    }

    delete[] old_array;
}
