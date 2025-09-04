#include "File.hpp"
#include <iostream>
#include <ctime>


void File::clean(TreeNode* node){
    if(node == nullptr){
        return;
    }
    for(auto child : node->children){
        clean(child);
    }
    delete node;
}
File::File(const string& name, HashMap& v_map) : version_map(v_map), filename(name), total_versions(1){
    root = new TreeNode(0, "", "Initial Snapshot for root", nullptr, time(0), time(0));
    active_version = root;
    version_map.insert(0, root);
}
File::~File(){
    clean(root);
}
void File::read() const{
    cout << active_version->content << endl;
}
void File::insert_content(const string& new_content){
    if(active_version->snapshot_timestamp == (time_t)0){
        active_version->content += new_content;
    }
    else{
        TreeNode* new_version = new TreeNode(total_versions, 
                                            active_version->content + new_content, 
                                            "", 
                                            active_version, 
                                            time(0), 
                                            (time_t)0);
        active_version->children.push_back(new_version);
        active_version = new_version;
        version_map.insert(total_versions, new_version);
        total_versions++;
    }
}
void File::update_content(const string& upd_content){
    if(active_version->snapshot_timestamp == (time_t)0){
        active_version->content = upd_content;
    }
    else{
        TreeNode* new_version = new TreeNode(total_versions, 
                                            upd_content, 
                                            "", 
                                            active_version, 
                                            time(0), 
                                            (time_t)0);
        active_version->children.push_back(new_version);
        active_version = new_version;
        version_map.insert(total_versions, new_version);
        total_versions++;
    }
}
void File::snapshot(const string& msg){
    if(active_version->snapshot_timestamp != (time_t)0){
        cout << "Snapshot already exists for the current version." << endl;
        return;
    }
    else{
        active_version->snapshot_timestamp = time(0);
        active_version->message = msg;
    }
}
bool File::rollback(const int& v_id){
    if(v_id == -1){
        if(active_version->parent == nullptr){
            cout << "Error: No parent version to rollback to." << endl;
            return false;
        }
        active_version = active_version->parent;
        return true;
    }
    TreeNode* target_version = version_map.get(v_id);
    if(target_version == nullptr){
        cout << "Error: Version ID " << v_id << " " << "does not exist." << endl;
        return false;
    }
    active_version = target_version;
    return true;
}
void File::history(){
    vector<TreeNode*> snapshots;
    for(auto curr = active_version; curr != nullptr; curr = curr->parent){
        if(curr->snapshot_timestamp != (time_t)0){
            snapshots.push_back(curr);
        }
    }
    reverse(snapshots.begin(), snapshots.end());
    for(auto const& node: snapshots){
        cout << "Version ID: " << node->version_id << ", Message: " << node->message << endl;
        cout << "Created At: " << ctime(&node->created_timestamp); 
        cout << "Snapshot At: " << ctime(&node->snapshot_timestamp);
    }
}