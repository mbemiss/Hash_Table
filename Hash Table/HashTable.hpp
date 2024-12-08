// HashTable.hpp

#include "HashTable.h"

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