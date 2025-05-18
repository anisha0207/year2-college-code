#pragma once
#include <cstddef>  // for size_t

class HashSet {
private:
    struct Node {
        int data;  // CHANGED: Store integers instead of strings
        Node* next;
        Node(int val) : data(val), next(nullptr) {}  // CHANGED: Constructor accepts integers
    };

    Node** array;  // Array of pointers to linked lists (buckets)
    size_t bucket_count;      // Number of buckets in the hash table
    size_t element_count;     // Total number of elements in the set
    unsigned int load_threshold;  // Maximum load factor before resizing occurs
    unsigned int load_factor;     // Current load factor of the hash set

    void updateLoadFactor();
    void rehash(size_t new_size);

public:
    explicit HashSet(size_t initial_size);  // Initialize an empty hash set with a given number of buckets
    ~HashSet();                             // Destructor: Free all allocated memory

    unsigned long prehash(int item) const;  // CHANGED: Generate a prehash for an integer item
    unsigned long hash(unsigned long prehash) const;  // Convert prehash value into a valid bucket index

    bool insert(int item);          // CHANGED: Insert integer into the set
    bool remove(int item);          // CHANGED: Remove an integer from the set
    bool contains(int item) const;  // CHANGED: Check if an integer exists in the set

    size_t count() const;           // Return the number of elements in the hash set
    unsigned int load() const;      // Return the current load factor as a percentage
    void set_load_threshold(unsigned int threshold);  // Set a new load factor threshold for resizing
    void clear();                   // Remove all elements from the hash set
};
