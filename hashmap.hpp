#ifndef HASHMAP_HPP
#define HASHMAP_HPP

#include <string>
#include <vector>
#include <stdexcept>
#include <functional>
#include "program.hpp"

//later adjusted and implemented from my midterm 2 hashmap
// Simple hash map implementation that uses separate chaining
class HashMap {
private:
    // Node structure for linked list in each bucket
    struct Node {
        std::string key;
        File value;
        Node* next;
        
        Node(const std::string& k, const File& v) : key(k), value(v), next(nullptr) {}
    };
    
    // Array of linked lists (buckets)
    std::vector<Node*> buckets;
    size_t size;
    size_t capacity;
    
    // Hash function
    size_t hash(const std::string& key) const {
        // Simple hash function
        std::hash<std::string> hasher;
        return hasher(key) % capacity;
    }

public:
    // Constructor
    HashMap(size_t initialCapacity = 16) : buckets(initialCapacity, nullptr), size(0), capacity(initialCapacity) {}
    
    // Destructor
    ~HashMap() {
        clear();
    }
    
    // Insert a key-value pair
    // Returns true if the key already existed (and was replaced), false otherwise
    bool insert(const std::string& key, const File& value) {
        size_t index = hash(key);
        
        // Check if key already exists
        Node* current = buckets[index];
        while (current != nullptr) {
            if (current->key == key) {
                // Replace existing value
                current->value = value;
                return true; // Key already existed
            }
            current = current->next;
        }
        
        // Insert new node at front of list
        Node* newNode = new Node(key, value);
        newNode->next = buckets[index];
        buckets[index] = newNode;
        size++;
        
        // Check if we need to resize
        if (size > capacity * 0.75) {
            resize(capacity * 2);
        }
        
        return false; // Key did not exist
    }
    
    // Get value for a key
    // Throws exception if key doesn't exist
    File get(const std::string& key) const {
        size_t index = hash(key);
        
        Node* current = buckets[index];
        while (current != nullptr) {
            if (current->key == key) {
                return current->value;
            }
            current = current->next;
        }
        
        throw std::out_of_range("Key not found in hash map: " + key);
    }
    
    // Check if a key exists
    bool contains(const std::string& key) const {
        size_t index = hash(key);
        
        Node* current = buckets[index];
        while (current != nullptr) {
            if (current->key == key) {
                return true;
            }
            current = current->next;
        }
        
        return false;
    }
    
    // Get all keys in the hash map
    std::vector<std::string> keys() const {
        std::vector<std::string> result;
        result.reserve(size);
        
        for (size_t i = 0; i < capacity; i++) {
            Node* current = buckets[i];
            while (current != nullptr) {
                result.push_back(current->key);
                current = current->next;
            }
        }
        
        return result;
    }
    
    // Get all values in the hash map
    std::vector<File> values() const {
        std::vector<File> result;
        result.reserve(size);
        
        for (size_t i = 0; i < capacity; i++) {
            Node* current = buckets[i];
            while (current != nullptr) {
                result.push_back(current->value);
                current = current->next;
            }
        }
        
        return result;
    }
    
    // Clear the hash map
    void clear() {
        for (size_t i = 0; i < capacity; i++) {
            Node* current = buckets[i];
            while (current != nullptr) {
                Node* next = current->next;
                delete current;
                current = next;
            }
            buckets[i] = nullptr;
        }
        size = 0;
    }
    
    // Resize the hash map
    void resize(size_t newCapacity) {
        std::vector<Node*> oldBuckets = std::move(buckets);
        size_t oldCapacity = capacity;
        
        // Initialize new buckets
        buckets = std::vector<Node*>(newCapacity, nullptr);
        capacity = newCapacity;
        size = 0;
        
        // Re-insert all nodes
        for (size_t i = 0; i < oldCapacity; i++) {
            Node* current = oldBuckets[i];
            while (current != nullptr) {
                Node* next = current->next;
                
                // Re-hash and insert
                size_t newIndex = hash(current->key);
                current->next = buckets[newIndex];
                buckets[newIndex] = current;
                size++;
                
                current = next;
            }
        }
    }
    
    // Return number of elements
    size_t getSize() const {
        return size;
    }
    
    // Return capacity
    size_t getCapacity() const {
        return capacity;
    }
};

#endif // HASHMAP_HPP