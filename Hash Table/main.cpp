/* 
This file contains the main function and performance testing code for the HashTable implementation.
It includes performance tests for both the custom HashTable and the standard (std::)unordered_map,
measuring the time taken for insert, retrieve, and remove operations with varying numbers of elements.
The results are output to the console, allowing for comparison between the custom implementation
and the standard library container.
*/

#include <iostream> // Include iostream for console output
#include <chrono> // Include chrono for time measurement
#include <random> // Include random for random number generation
#include <unordered_map> // Include unordered_map for standard hash table
#include "HashTable.h" // Include custom HashTable implementation

using namespace std; // using standard namespace to avoid writing std:: before standard library functions
using namespace chrono; // using chrono namespace to avoid writing chrono:: before time measurement functions

// Performance test for custom HashTable
template <typename K, typename V> // Template for key-value pair types
void performanceTestCustom(HashTable<K, V>& hashTable, int operations) { // Performance test method
	random_device rd; // Random device for seed
	mt19937 gen(rd()); // Mersenne Twister engine for random number generation
	uniform_int_distribution<> dis(1, 1000000); // Uniform distribution for random numbers

	auto start = high_resolution_clock::now(); // Start time measurement

	int successfulInserts = 0; // Counter for successful inserts
	int successfulRetrieves = 0; // Counter for successful retrieves
	int successfulRemoves = 0; // Counter for successful removes

	for (int i = 0; i < operations; ++i) { // Loop for insert operations
		int key = dis(gen); // Generate random key
		int value = dis(gen); // Generate random value
		try { // Try to insert key-value pair
			hashTable.insert(key, value); // Insert key-value pair
			successfulInserts++; // Increment successful inserts counter
        }
		catch (const exception& e) { // Catch any exceptions
			cerr << "Error during insertion: " << e.what() << endl; // Output error message
			break; // Break out of loop
        }
    }

	for (int i = 0; i < operations; ++i) { // Loop for retrieve operations
		int key = dis(gen); // Generate random key
		try { // Try to retrieve value for key
			hashTable.retrieve(key); // Retrieve value for key
			successfulRetrieves++; // Increment successful retrieves counter
        }
		catch (const runtime_error&) { // Key not found, ignore for performance reasons
			// Uncomment the following line for debugging, but note it will significantly slow down the program and print a lot to screen
			//cerr << "Key not found: " << key << endl;		
        }
		catch (const exception& e) { // Catch any other exceptions
			cerr << "Error during retrieval: " << e.what() << endl; // Output error message
        }
    }

	for (int i = 0; i < operations; ++i) { // Loop for remove operations
		int key = dis(gen); // Generate random key
		try { // Try to remove key
			hashTable.remove(key); // Remove key
			successfulRemoves++; // Increment successful removes counter
        }
		catch (const exception& e) { // Catch any exceptions
			cerr << "Error during removal: " << e.what() << endl; // Output error message
        }
    }

	auto end = high_resolution_clock::now(); // End time measurement
	auto duration = duration_cast<milliseconds>(end - start); // Calculate duration
	cout << "Time taken for " << operations << " operations: " << duration.count() << " ms" << endl; // Output time taken
	cout << "Successful inserts: " << successfulInserts << endl; // Output successful inserts
	cout << "Successful retrieves: " << successfulRetrieves << endl; // Output successful retrieves
	cout << "Successful removes: " << successfulRemoves << endl; // Output successful removes
	cout << "Final hash table size: " << hashTable.getSize() << endl; // Output final size
	cout << "Final hash table count: " << hashTable.getCount() << endl; // Output final count
}

// Performance test for standard unordered_map
void performanceTestStd(unordered_map<int, int>& hashTable, int operations) { // Performance test method
	random_device rd; // Random device for seed
	mt19937 gen(rd()); // Mersenne Twister engine for random number generation
	uniform_int_distribution<> dis(1, 1000000); // Uniform distribution for random numbers

	auto start = high_resolution_clock::now(); // Start time measurement

	for (int i = 0; i < operations; ++i) { // Loop for insert operations
		int key = dis(gen); // Generate random key
		int value = dis(gen); // Generate random value
		hashTable.insert({ key, value }); // Insert key-value pair
    }

	for (int i = 0; i < operations; ++i) { // Loop for retrieve operations
		int key = dis(gen); // Generate random key
		auto it = hashTable.find(key); // Find key in hash table
		if (it != hashTable.end()) { // If key is found
			// Uncomment the following line for debugging, but note it will significantly slow down the program and print a LOT of output to screen
			//cout << "Retrieved value for key " << key << ": " << it->second << endl;            
        }
    }

	for (int i = 0; i < operations; ++i) { // Loop for remove operations
		int key = dis(gen); // Generate random key
		hashTable.erase(key); // Remove key from hash table
    }

	auto end = high_resolution_clock::now(); // End time measurement
	auto duration = duration_cast<milliseconds>(end - start); // Calculate duration
	cout << "Time taken for " << operations << " operations: " << duration.count() << " ms" << endl; // Output time taken
}

// Main function: Runs performance tests on custom HashTable and standard unordered_map
int main() { // Main function
	HashTable<int, int> customHashTable; // Custom HashTable instance
	unordered_map<int, int> stdHashTable; // Standard unordered_map instance

	cout << "Custom Hash Table Performance:" << endl; // Output custom hash table performance message
	performanceTestCustom(customHashTable, 100); // Perform custom hash table test with 100 operations
	performanceTestCustom(customHashTable, 1000); // Perform custom hash table test with 1000 operations
	performanceTestCustom(customHashTable, 10000); // Perform custom hash table test with 10000 operations

	cout << "\nStandard Hash Table Performance:" << endl; // Output standard hash table performance message
	performanceTestStd(stdHashTable, 100); // Perform standard hash table test with 100 operations
	performanceTestStd(stdHashTable, 1000); // Perform standard hash table test with 1000 operations
	performanceTestStd(stdHashTable, 10000); // Perform standard hash table test with 10000 operations

	return 0; // Return 0 to indicate successful completion
}