#include "TreeNode.hpp"

TreeNode::TreeNode(int id, const string& cont, 
        const string& msg, 
        TreeNode* par, 
        time_t curr_time, 
        time_t snapshot_time,
        time_t last_mod_time, 
        const vector<TreeNode*>& childs):
        version_id(id), content(cont), message(msg),
        created_timestamp(curr_time), snapshot_timestamp(snapshot_time), last_modified_time(last_mod_time),
        parent(par), children(childs){
        }