#include "HashMap_File.hpp"

template<typename T>
HashMap_File<T>::HashNode::HashNode(string k, T v){
    key = k;
    value = v;
    next = nullptr;
}
template<typename T>
unsigned long HashMap_File<T>::hash_function(const string& key){
    unsigned long hash = 5381;
    for(char c : key){
        hash = ((hash << 5) + hash) + c;
    }
    return hash % max_size;
}
template<typename T>
void HashMap_File<T>::rehash(){  
    int old_max = max_size;
    max_size *= 2;
    vector<HashNode*> new_table(max_size, nullptr);
    for(int i = 0; i < old_max; i++){
        HashNode* node = table[i];
        while(node != nullptr){
            HashNode* next_node = node->next;
            unsigned long idx = hash_function(node->key);
            node->next =   new_table[idx];
            new_table[idx] = node;
            node = next_node;
        }
    }
    table = std::move(new_table); // efficient transfer (creates an rvalue reference)
}
template<typename T>
HashMap_File<T>::HashMap_File(int initial_size){
    max_size = initial_size;
    curr_size = 0;
    table.resize(max_size, nullptr);
}
template<typename T>
HashMap_File<T>::~HashMap_File(){
    for(auto front: table){
        while(front != NULL){
            HashNode* temp = front;
            front = front->next;
            delete temp;
        }
    }
}
template<typename T>
void HashMap_File<T>::insert(const string& key, T value){
    unsigned long idx = hash_function(key);
    HashNode* node = table[idx];
    while(node != nullptr){
        if(node->key == key){
            node->value = value;
            return;
        }
        node = node->next;
    }
    HashNode* new_node = new HashNode(key, value);
    new_node->next = table[idx];
    table[idx] = new_node;
    curr_size++;
    if((double)curr_size / max_size > load_factor){
        rehash();
    }
}
template<typename T>
T HashMap_File<T>::get(const string& key){
    unsigned long idx = hash_function(key);
    HashNode* node = table[idx];
    while(node != nullptr){
        if(node->key == key){
            return node->value;
        }
        node = node->next;
    }
    return T();
}
template<typename T>
bool HashMap_File<T>::remove(const string& key){
    unsigned long idx = hash_function(key);
    HashNode* node = table[idx];
    HashNode* prev = nullptr;
    while(node != nullptr){
        if(node->key == key){
            if(prev == nullptr){
                table[idx] = node->next;
            }
            else{
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