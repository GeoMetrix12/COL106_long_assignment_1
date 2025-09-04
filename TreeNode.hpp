#ifndef TREENODE_HPP
#define TREENODE_HPP

#include<string>
#include<vector>
#include<ctime>
using namespace std;

class TreeNode{
    public:
        int version_id;
        string content;
        string message;
        time_t created_timestamp;
        time_t snapshot_timestamp;
        time_t last_modified_time;
        TreeNode* parent;
        vector<TreeNode*> children;
        TreeNode(int id, const string& cont  = "", 
                const string& msg = "", 
                TreeNode* par = nullptr, 
                time_t curr_time = time(0), 
                time_t snapshot_time = (time_t)0, 
                time_t last_mod_time = time(0),
                const vector<TreeNode*>& childs = vector<TreeNode*>());
};
#endif 