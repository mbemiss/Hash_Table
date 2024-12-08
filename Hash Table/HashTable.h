// HashTable.h

#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include <vector>
#include <list>
#include <stdexcept>
#include <functional>

template <typename K, typename V>
class HashTable {
private:
    static const int DEFAULT_SIZE = 10;
    std::vector<std::list<std::pair<K, V>>> table;
    int size;

    int hash(const K& key) const {
        return std::hash<K>{}(key) % size;
    }

public:
    HashTable(int s = DEFAULT_SIZE) : size(s), table(s) {}

    void insert(const K& key, const V& value);
    V retrieve(const K& key) const;
    void remove(const K& key);
};

// Implementation

template <typename K, typename V>
void HashTable<K, V>::insert(const K& key, const V& value) {
    int index = hash(key);
    for (auto& pair : table[index]) {
        if (pair.first == key) {
            pair.second = value;
            return;
        }
    }
    table[index].push_back(std::make_pair(key, value));
}

template <typename K, typename V>
V HashTable<K, V>::retrieve(const K& key) const {
    int index = hash(key);
    for (const auto& pair : table[index]) {
        if (pair.first == key) {
            return pair.second;
        }
    }
    throw std::runtime_error("Key not found");
}

template <typename K, typename V>
void HashTable<K, V>::remove(const K& key) {
    int index = hash(key);
    auto& list = table[index];
    for (auto it = list.begin(); it != list.end(); ++it) {
        if (it->first == key) {
            list.erase(it);
            return;
        }
    }
}

#endif // HASH_TABLE_H