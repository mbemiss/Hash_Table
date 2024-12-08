// HashTable.h

#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <vector>
#include <stdexcept>
#include <functional>
#include <iostream>

template <typename K, typename V>
class HashTable {
private:
    static const int DEFAULT_SIZE = 10;
    std::vector<std::pair<K, V>> table;
    std::vector<bool> occupied;
    int size;
    int count;

    int hash1(const K& key) const {
        return std::hash<K>{}(key) % size;
    }

    int hash2(const K& key) const {
        return 1 + (std::hash<K>{}(key) % (size - 1));
    }

    int doubleHash(const K& key, int i) const {
        int index = (hash1(key) + i * hash2(key)) % size;
        if (index < 0 || index >= size) {
            throw std::out_of_range("Double hash produced an out-of-range index");
        }
        return index;
    }

public:
    HashTable(int s = DEFAULT_SIZE) : size(s), table(s), occupied(s, false), count(0) {}

    void insert(const K& key, const V& value);
    V retrieve(const K& key) const;
    void remove(const K& key);
    int getSize() const { return size; }
    int getCount() const { return count; }
};

template <typename K, typename V>
void HashTable<K, V>::insert(const K& key, const V& value) {
    if (count >= size / 2) {
        throw std::runtime_error("Hash table is more than half full");
    }

    int i = 0;
    while (i < size) {  // Add a limit to prevent infinite loop
        try {
            int index = doubleHash(key, i);
            if (!occupied.at(index) || table.at(index).first == key) {
                table.at(index) = std::make_pair(key, value);
                if (!occupied.at(index)) {
                    occupied.at(index) = true;
                    count++;
                }
                return;
            }
        }
        catch (const std::out_of_range& e) {
            std::cerr << "Error in insert: " << e.what() << std::endl;
            throw;
        }
        i++;
    }
    throw std::runtime_error("Failed to insert: Hash table is full");
}

template <typename K, typename V>
V HashTable<K, V>::retrieve(const K& key) const {
    int i = 0;
    while (i < size) {  // Add a limit to prevent infinite loop
        try {
            int index = doubleHash(key, i);
            if (!occupied.at(index)) {
                throw std::runtime_error("Key not found");
            }
            if (table.at(index).first == key) {
                return table.at(index).second;
            }
        }
        catch (const std::out_of_range& e) {
            std::cerr << "Error in retrieve: " << e.what() << std::endl;
            throw;
        }
        i++;
    }
    throw std::runtime_error("Key not found");
}

template <typename K, typename V>
void HashTable<K, V>::remove(const K& key) {
    int i = 0;
    while (i < size) {  // Add a limit to prevent infinite loop
        try {
            int index = doubleHash(key, i);
            if (!occupied.at(index)) {
                return;  // Key not found, do nothing
            }
            if (table.at(index).first == key) {
                occupied.at(index) = false;
                count--;
                return;
            }
        }
        catch (const std::out_of_range& e) {
            std::cerr << "Error in remove: " << e.what() << std::endl;
            throw;
        }
        i++;
    }
}

#endif // HASH_TABLE_H