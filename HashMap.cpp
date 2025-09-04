#include "HashMap.hpp"

HashMap::HashNode::HashNode(int k, TreeNode* v) : key(k), value(v), next(nullptr) {}

int HashMap::hash_function(int key) {
    return key % max_size;
}

void HashMap::rehash() {
    int old_max = max_size;
    max_size *= 2;
    std::vector<HashNode*> new_table(max_size, nullptr);
    for (int i = 0; i < old_max; i++) {
        HashNode* node = table[i];
        while (node != nullptr) {
            HashNode* next_node = node->next;
            int idx = hash_function(node->key);
            node->next = new_table[idx];
            new_table[idx] = node;
            node = next_node;
        }
    }
    table = std::move(new_table);
}

HashMap::HashMap(int initial_size) : curr_size(0), max_size(initial_size) {
    table.resize(max_size, nullptr);
}

HashMap::~HashMap() {
    for (auto front : table) {
        while (front != nullptr) {
            HashNode* temp = front;
            front = front->next;
            delete temp;
        }
    }
}

void HashMap::insert(int key, TreeNode* value) {
    int idx = hash_function(key);
    HashNode* node = table[idx];
    while (node != nullptr) {
        if (node->key == key) {
            node->value = value;
            return;
        }
        node = node->next;
    }
    HashNode* new_node = new HashNode(key, value);
    new_node->next = table[idx];
    table[idx] = new_node;
    curr_size++;
    if ((double)curr_size / max_size > load_factor) {
        rehash();
    }
}

TreeNode* HashMap::get(int key) {
    int idx = hash_function(key);
    HashNode* node = table[idx];
    while (node != nullptr) {
        if (node->key == key) {
            return node->value;
        }
        node = node->next;
    }
    return nullptr;
}

bool HashMap::remove(int key) {
    int idx = hash_function(key);
    HashNode* node = table[idx];
    HashNode* prev = nullptr;
    while (node != nullptr) {
        if (node->key == key) {
            if (prev == nullptr) {
                table[idx] = node->next;
            } else {
                prev->next = node->next;
            }
            delete node;
            curr_size--;
            return true;
        }
        prev = node;
        node = node->next;
    }
    return false;
}
