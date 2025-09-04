#ifndef HASHMAP_HPP
#define HASHMAP_HPP

#include "TreeNode.hpp"
#include<vector>

class HashMap{
    private:
        class HashNode{
            public: 
                int key;
                TreeNode* value;
                HashNode* next;
                HashNode(int k, TreeNode* v);
        };

        vector<HashNode*> table;
        int curr_size;
        int max_size;

        static constexpr double load_factor = 0.75;

        int hash_function(int key);

        void rehash();
    public:
        HashMap(int initial_size = 16);
        ~HashMap();
        void insert(int key, TreeNode* value);
        TreeNode* get(int key);
        bool remove(int key);
};

#endif