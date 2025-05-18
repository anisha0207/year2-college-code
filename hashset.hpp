#ifndef HASHMAP_H
#define HASHMAP_H

#include <vector>
#include <list>
#include <string>
#include <stdexcept>

// KeyValuePair Class
/**
 * @brief A class representing a key-value pair.
 * 
 * This class is used to store a pair consisting of an integer key and a string value.
 */
class KeyValuePair {
private:
    int key;               // The key (integer)
    std::string value;     // The value (string)

public:
    // Constructor: Initializes the key-value pair
    KeyValuePair(int key, const std::string& value) : key(key), value(value) {}

    // Destructor
    ~KeyValuePair() {}

    // Getters
    int getKey() const { 
        return key; 
    }
    std::string getValue() const { 
        return value; 
    }

    // Setters
    void setKey(int newKey) { 
        key = newKey; 
    }
    void setValue(const std::string& newValue) { 
        value = newValue; 
    }
};

// HashMap Class
/**
 * This class uses a bucket array to store key-value pairs and handles collisions
 * using separate chaining (linked lists).
 */
class HashMap {
private:
    std::vector<std::list<KeyValuePair>> buckets; // Bucket array with linked lists
    size_t bucketCount;                           // Number of buckets

    /**
     * @param key The integer key to hash.
     * @return The index of the bucket corresponding to the given key.
     */
    size_t hashFunction(int key) const {
        return key % bucketCount;
    }

public:
    // Constructor: Initializes the bucket array with a default size of 10
    HashMap(size_t bucketCount = 10) : buckets(bucketCount), bucketCount(bucketCount) {}

    // Destructor
    ~HashMap() {}

    /**
     * @param key The integer key.
     * @param value The string value associated with the key.
     * @return True if insertion/update was successful, false otherwise.
     */
    bool insert(int key, const std::string& value);

    /**
     * @param key The integer key to remove.
     * @return True if the removal was successful, false if the key does not exist.
     */
    bool remove(int key);

    /**
     * @param key The integer key to search for.
     * @param value_out A reference to store the retrieved value if found.
     * @return True if the key exists and the value is retrieved, false otherwise.
     */
    bool get(int key, std::string& value_out);
};

#endif // HASHMAP_H
