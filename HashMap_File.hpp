#ifndef HASHMAP_FILE_HPP
#define HASHMAP_FILE_HPP

#include "File.hpp"
#include <string>
#include <vector>

class HashMap_File{
    private:
        class HashNode{
            public: 
                string key;
                File* value;
                HashNode* next;
                HashNode(string k, File* v);
        };

        vector<HashNode*> table;
        int curr_size;
        int max_size;

        static constexpr double load_factor = 0.75;

        unsigned long hash_function(const string& key);

        void rehash();
    public:
        HashMap_File(int initial_size = 16);
        ~HashMap_File();
        void insert(const string& key, File* value);
        File* get(const string& key);
        bool remove(const string& key);
};

#endif