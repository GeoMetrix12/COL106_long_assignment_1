#include "HashMap_File.hpp"
HashMap_File::HashNode::HashNode(string k, File* v){
    key = k;
    value = v;
    next = nullptr;
}
unsigned long HashMap_File::hash_function(const string& key){
    unsigned long hash = 5381;
    for(char c : key){
        hash = ((hash << 5) + hash) + c;
    }
    return hash % max_size;
}
void HashMap_File::rehash(){  
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
HashMap_File::HashMap_File(int initial_size){
    max_size = initial_size;
    curr_size = 0;
    table.resize(max_size, nullptr);
}
HashMap_File::~HashMap_File(){
    for(auto front: table){
        while(front != NULL){
            HashNode* temp = front;
            front = front->next;
            delete temp;
        }
    }
}
void HashMap_File::insert(const string& key, File* value){
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
File* HashMap_File::get(const string& key){
    unsigned long idx = hash_function(key);
    HashNode* node = table[idx];
    while(node != nullptr){
        if(node->key == key){
            return node->value;
        }
        node = node->next;
    }
    return nullptr;
}
bool HashMap_File::remove(const string& key){
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