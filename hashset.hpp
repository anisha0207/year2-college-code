#pragma once
#include <cstddef>     // for size_t
#include <string>      // for std::string

class HashSet {
private:
    struct Node {
        std::string data;
        Node* next;
        Node(const std::string& val) : data(val), next(nullptr) {}
    };

    Node** array;
    // member variables of the HashSet class, 
    // bucket_count: Stores the number of buckets in the hash table.
    // element_count: Keeps track of the total number of elements in the set.
    // load_threshold: Represents the maximum load factor before resizing occurs.
    // load_factor: Stores the current load factor of the hash set
    size_t bucket_count;
    size_t element_count;
    unsigned int load_threshold;
    unsigned int load_factor;

    void updateLoadFactor();
    void rehash(size_t new_size);

public:
    // Initialize an empty hash set with a given number of buckets
    explicit HashSet(size_t initial_size);

    // Destructor: Free all allocated memory
    ~HashSet();

    // Generate a prehash for an item
    unsigned long prehash(const std::string& item) const;

    // Convert prehash value into a valid bucket index
    unsigned long hash(unsigned long prehash) const;

    // Insert item into the set. Returns true if inserted, false if already present.
    // Rehashes if inserting will increase the load factor past the threshold.
    bool insert(const std::string& item);

    // Remove an item from the set. Returns true if removed, false if not found.
    bool remove(const std::string& item);

    // Check if the item exists in the set
    bool contains(const std::string& item) const;

    // Return the number of elements in the hash set
    size_t count() const;

    // Return the current load factor as a percentage
    unsigned int load() const;

    // Set a new load factor threshold for resizing
    void set_load_threshold(unsigned int threshold);

    // Remove all elements from the hash set
    void clear();

    //Had debugging issues and ended up changing this file to where we intialized the Node and its array here rather than in the lib.c
    //first few codes were moved from lib.c to this file for convenience
};
