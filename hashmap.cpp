#include "hashset.hpp"
#include <iostream>

// Implementation of HashMap member functions

/**
 * @param key The integer key.
 * @param value The string value associated with the key.
 * @return True if insertion/update was successful, false otherwise.
 */
bool HashMap::insert(int key, const std::string& value) {
    size_t index = hashFunction(key);

    // Check if the key already exists in the bucket
    for (auto& pair : buckets[index]) {
        if (pair.getKey() == key) {
            pair.setValue(value); // Update existing value
            return true;
        }
    }

    // If not found, insert a new KeyValuePair into the bucket
    buckets[index].emplace_back(key, value);
    return true;
}

/**
* @param key The integer key to remove.
* @return True if removal was successful, false if the key does not exist.
*/
bool HashMap::remove(int key) {
   size_t index = hashFunction(key);


   auto& bucket = buckets[index];//to avoid creating a copy of the elements inside the vector within the thread variable
   for (auto it = bucket.begin(); it != bucket.end(); ++it) {
       if (it->getKey() == key) {
           bucket.erase(it); // Remove the KeyValuePair from the bucket
           return true;
       }
   }


   return false; // Key not found
}

/**
* @param key The integer key to search for.
* @param value_out A reference to store the retrieved value if found.
* @return True if the key exists and the value is retrieved, false otherwise.
*/
bool HashMap::get(int key, std::string& value_out) {
   size_t index = hashFunction(key);


   for (const auto& pair : buckets[index]) {
       if (pair.getKey() == key) {
           value_out = pair.getValue(); // Retrieve the associated value
           return true;
       }
   }


   return false; // Key not found
}
