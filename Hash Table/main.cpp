// main.cpp

#include <iostream>
#include <chrono>
#include <random>
#include <unordered_map>
#include "HashTable.h"

// For custom HashTable
template <typename K, typename V>
void performanceTestCustom(HashTable<K, V>& hashTable, int operations) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 1000000);

    auto start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < operations; ++i) {
        int key = dis(gen);
        int value = dis(gen);
        hashTable.insert(key, value);
    }

    for (int i = 0; i < operations; ++i) {
        int key = dis(gen);
        try {
            hashTable.retrieve(key);
        }
        catch (const std::runtime_error&) {
            // Key not found, ignore
        }
    }

    for (int i = 0; i < operations; ++i) {
        int key = dis(gen);
        hashTable.remove(key);
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "Time taken for " << operations << " operations: " << duration.count() << " ms" << std::endl;
}

// For std::unordered_map
void performanceTestStd(std::unordered_map<int, int>& hashTable, int operations) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 1000000);

    auto start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < operations; ++i) {
        int key = dis(gen);
        int value = dis(gen);
        hashTable.insert({ key, value });
    }

    for (int i = 0; i < operations; ++i) {
        int key = dis(gen);
        auto it = hashTable.find(key);
        if (it != hashTable.end()) {
            // Key found, you can access the value with it->second
        }
    }

    for (int i = 0; i < operations; ++i) {
        int key = dis(gen);
        hashTable.erase(key);
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "Time taken for " << operations << " operations: " << duration.count() << " ms" << std::endl;
}

int main() {
    HashTable<int, int> customHashTable;
    std::unordered_map<int, int> stdHashTable;

    std::cout << "Custom Hash Table Performance:" << std::endl;
    performanceTestCustom(customHashTable, 100);
    performanceTestCustom(customHashTable, 1000);
    performanceTestCustom(customHashTable, 10000);

    std::cout << "\nStandard Hash Table Performance:" << std::endl;
    performanceTestStd(stdHashTable, 100);
    performanceTestStd(stdHashTable, 1000);
    performanceTestStd(stdHashTable, 10000);

    return 0;
}