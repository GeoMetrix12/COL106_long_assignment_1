#ifndef FILE_HPP
#define FILE_HPP

#include "HashMap.hpp"
#include "TreeNode.hpp"
#include <string>

class File{
    private:
        HashMap& version_map;
        TreeNode* root;
        void clean(TreeNode* node);
    public: 
        string filename;
        TreeNode* active_version;
        int total_versions;
        File(const string& name, HashMap& v_map);
        ~File();
        void read() const;
        void insert_content(const string& new_content);
        void update_content(const string& upd_content);
        void snapshot(const string& msg = "");
        bool rollback(const int& v_id = -1);
        void history();
};

#endif