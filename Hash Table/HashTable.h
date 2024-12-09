/* 
This file implements a templated HashTable class using double hashing for collision resolution.
It includes methods for inserting, retrieving, and removing key-value pairs, as well as dynamic
resizing to maintain performance. The implementation uses open addressing and handles collisions
through a custom double hashing function, with additional utility methods for debugging and
performance analysis.
*/

#ifndef HASH_TABLE_H // Include guard to prevent duplicate definitions
#define HASH_TABLE_H // This line defines the header file if it has not been defined before

#include <vector> // Include vector for dynamic array storage
#include <stdexcept> // Include stdexcept for runtime_error
#include <functional> // Include functional for hash
#include <iostream> // Include iostream for console output

using namespace std; // Using standard namespace to avoid writing std:: before standard library functions

//This section initializes the hash table with a given size or default size.
template <typename K, typename V> // Template for key-value pair types
class HashTable { // HashTable class definition
private: // Private section for internal data and helper methods
	static const int DEFAULT_SIZE = 10; // Default size for the hash table
	vector<pair<K, V>> table; // Vector to store key-value pairs
	vector<bool> occupied; // Vector to track occupied slots
	int size; // Current size of the hash table
	int count; // Number of elements in the hash table

	int hash1(const K& key) const { // First hash function using modulo
		return hash<K>{}(key) % size; // Hash the key and take modulo of size
    }

	int hash2(const K& key) const { // Second hash function using modulo and prime
		return 1 + (hash<K>{}(key) % (size - 1)); // Hash the key and take modulo of size - 1, add 1
    }

	int doubleHash(const K& key, int i) const { // Double hashing function
		int h1 = hash1(key); // First hash value
		int h2 = hash2(key); // Second hash value
		return (h1 + i * (h2 | 1)) % size;  // Double hashing formula.  Ensure h2 is odd
    }

	void resize() { // Resize method to increase the size of the hash table
		vector<pair<K, V>> oldTable = table; // Store old table
		vector<bool> oldOccupied = occupied; // Store old occupied vector
		int oldSize = size; // Store old size
		size *= 2; // Double the size
		table = vector<pair<K, V>>(size); // Create new table with new size
		occupied = vector<bool>(size, false); // Create new occupied vector with new size
		count = 0; // Reset count to 0

		for (int i = 0; i < oldSize; ++i) { // Iterate over old table
			if (oldOccupied[i]) { // If slot is occupied
				insert(oldTable[i].first, oldTable[i].second); // Insert key-value pair into new table
            }
        }
    }

// This section defines the public interface of the HashTable class.
public: // Public section for external interface methods
	HashTable(int s = DEFAULT_SIZE) : size(s), table(s), occupied(s, false), count(0) {} // Constructor with default size

	void insert(const K& key, const V& value); // Method to insert key-value pair
	V retrieve(const K& key) const; // Method to retrieve value for key
	void remove(const K& key); // Method to remove key-value pair
	int getSize() const { return size; } // Getter method for size
	int getCount() const { return count; } // Getter method for count
	void print() const; // Method to print the contents of the hash table
	void clear(); // Method to clear the hash table
};

// This section adds a key-value pair to the hash table, resizing if necessary and handling collisions.
template <typename K, typename V> // Template for key-value pair types
void HashTable<K, V>::insert(const K& key, const V& value) { // Insert method implementation
	if (static_cast<double>(count) / size > 0.75) { // Check load factor
		cout << "Resizing from " << size << " to " << size * 2 << endl; // Output resize message
		resize(); // Resize the hash table
    }

	int i = 0; // Initialize probe count
	while (i < size) { // Iterate over table
		int index = doubleHash(key, i); // Calculate index using double hashing
		if (!occupied[index] || table[index].first == key) { // If slot is empty or key matches
			table[index] = make_pair(key, value); // Insert key-value pair
			if (!occupied[index]) { // If slot was empty
				occupied[index] = true; // Mark slot as occupied
				count++; // Increment count
            }
			return; // Exit loop
        }
		i++; // Increment probe count
    }
	cout << "Failed to insert key " << key << ". Table size: " << size << ", Count: " << count << endl; // Output failure message
	throw runtime_error("Hash table is full"); // Throw exception if table is full
}

// This section searches for a key and returns its associated value, throwing an exception if not found.
template <typename K, typename V> // Template for key-value pair types
V HashTable<K, V>::retrieve(const K& key) const { // Retrieve method implementation
	int i = 0; // Initialize probe count
	while (i < size) { // Iterate over table
		int index = doubleHash(key, i); // Calculate index using double hashing
		if (!occupied[index]) { // If slot is empty
			break;  // Key not found
        }
		if (table[index].first == key) { // If key matches
			return table[index].second; // Return value
        }
		i++; // Increment probe count
    }
	throw runtime_error("Key not found"); // Throw exception if key is not found
}

// This section deletes a key-value pair from the hash table and rehashes subsequent elements.
template <typename K, typename V> // Template for key-value pair types
void HashTable<K, V>::remove(const K& key) { // Remove method implementation
	int i = 0; // Initialize probe count
	while (i < size) { // Iterate over table
		int index = doubleHash(key, i); // Calculate index using double hashing
		if (!occupied[index]) {  // If slot is empty
			return;  // Key not found
        }
		if (table[index].first == key) { // If key matches
			occupied[index] = false; // Mark slot as empty
			count--; // Decrement count
			//cout << "Removed key " << key << ". New count: " << count << endl; // Output removal message.  Be careful, this line can slow down the program
			// and print a lot of output to screen.

			// Rehash the elements after the removed key
			int j = (index + 1) % size; // Start rehashing from next index
			while (occupied[j]) { // Iterate over occupied slots
				K rehashKey = table[j].first; // Store key for rehashing
				V rehashValue = table[j].second; // Store value for rehashing
				occupied[j] = false; // Mark slot as empty
				count--; // Decrement count
				insert(rehashKey, rehashValue); // Reinsert key-value pair
				j = (j + 1) % size; // Move to next index
            }
			return; // Exit loop
        }
		i++; // Increment probe count
    }
}

// This section displays all occupied slots in the hash table, including their keys and values.
template <typename K, typename V> // Template for key-value pair types
void HashTable<K, V>::print() const { // Print method implementation
	for (int i = 0; i < size; ++i) { // Iterate over table
		if (occupied[i]) { // If slot is occupied
			cout << "Index " << i << ": Key = " << table[i].first // Output key-value pair
				<< ", Value = " << table[i].second << endl; // Output key-value pair
        }
    }
	cout << "Size: " << size << ", Count: " << count << endl; // Output size and count
}

// This section resets the hash table to an empty state, maintaining its current size.
template <typename K, typename V> // Template for key-value pair types
void HashTable<K, V>::clear() { // Clear method implementation
	table = vector<pair<K, V>>(size); // Create new table with same size
	occupied = vector<bool>(size, false); // Create new occupied vector with same size
	count = 0; // Reset count to 0
}

#endif // End of include guard for HASH_TABLE_H