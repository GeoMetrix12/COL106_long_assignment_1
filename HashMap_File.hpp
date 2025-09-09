#ifndef HASHMAP_FILE_HPP
#define HASHMAP_FILE_HPP

#include <string>
#include <vector>
using namespace std;

template<typename T>
class HashMap_File{
    private:
        class HashNode{
            public: 
                string key;
                T value;
                HashNode* next;
                HashNode(string k, T v);
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
        void insert(const string& key, T value);
        T get(const string& key);
        bool remove(const string& key);
};

#endif