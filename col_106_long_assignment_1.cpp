#include<bits/stdc++.h>
#include<iomanip>
#include<iostream>
#include<sstream>
#include<ctime>
#include<utility>
#include<string>
// #include<queue>
// #include<stack>
// #include<set>
// #include<map>
// #include<tuple>
// #include<algorithm>
// #include<random>
#define ll long long 
#define fastio() ios_base::sync_with_stdio(false);cin.tie(NULL);cout.tie(NULL)
#define rep(i,s) for(int i=0; i<s ; i++)
#define f(i,a,b) for(int i(a); i<b ; i++)
// const ll N = 100005;
// const ll MAXN = 10000001;    
// const ll SQRT = 750;
// const ll MOD = 1e9 + 7;
// const ll a_max = 1000001;  
// const ll MODI = 998244353;
using namespace std;

class TreeNode{
    public:
        int version_id;
        string content;
        string message;
        time_t created_timestamp;
        time_t snapshot_timestamp;
        TreeNode* parent;
        vector<TreeNode*> children;
        TreeNode(int id, const string& cont  = "", 
                const string& msg = "", 
                TreeNode* par = nullptr, 
                time_t curr_time = time(0), 
                time_t snapshot_time = (time_t)0, 
                const vector<TreeNode*>& childs = vector<TreeNode*>()){
            version_id = id;
            parent = par;
            content = cont;
            message = "";
            created_timestamp = curr_time;
            snapshot_timestamp = snapshot_time;
            children = childs;
        }
};
class HashMap{
    private:
        class HashNode{
            public: 
                int key;
                TreeNode* value;
                HashNode* next;
                HashNode(int k, TreeNode* v){
                    key = k;
                    value = v;
                    next = nullptr;
                }
        };

        vector<HashNode*> table;
        int curr_size;
        int max_size;

        static constexpr double load_factor = 0.75;

        int hash_function(int key){
            return key % max_size;
        }

        void rehash(){  
            int old_max = max_size;
            max_size *= 2;
            vector<HashNode*> new_table(max_size, nullptr);
            rep(i, old_max){
                HashNode* node = table[i];
                while(node != nullptr){
                    HashNode* next_node = node->next;
                    int idx = hash_function(node->key);
                    node->next =   new_table[idx];
                    new_table[idx] = node;
                    node = next_node;
                }
            }
            table = std::move(new_table); // efficient transfer (creates an rvalue reference)
        }
    public:
        HashMap(int initial_size = 16){
            max_size = initial_size;
            curr_size = 0;
            table.resize(max_size, nullptr);
        }
        ~HashMap(){
            for(auto front: table){
                while(front != NULL){
                    HashNode* temp = front;
                    front = front->next;
                    delete temp;
                }
            }
        }
        void insert(int key, TreeNode* value){
            int idx = hash_function(key);
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
        TreeNode* get(int key){
            int idx = hash_function(key);
            HashNode* node = table[idx];
            while(node != nullptr){
                if(node->key == key){
                    return node->value;
                }
                node = node->next;
            }
            return nullptr;
        }
        bool remove(int key){
            int idx = hash_function(key);
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
};

class File{
    private:
        HashMap& version_map;
        TreeNode* root;
        void clean(TreeNode* node){
            if(node == nullptr){
                return;
            }
            for(auto child : node->children){
                clean(child);
            }
            delete node;
        }

    public: 
        string filename;
        TreeNode* active_version;
        int total_versions;
        File(const string& name, HashMap& v_map) : version_map(v_map), filename(name), total_versions(1){
            root = new TreeNode(0, "", "Initial Snapshot for root", nullptr, time(0), time(0));
            active_version = root;
            version_map.insert(0, root);
        }
        ~File(){
            clean(root);
        }
        void read() const{
            cout << active_version->content << endl;
        }
        void insert_content(const string& new_content){
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
        void update_content(const string& upd_content){
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
        void snapshot(const string& msg = ""){
            if(active_version->snapshot_timestamp != (time_t)0){
                cout << "Snapshot already exists for the current version." << endl;
                return;
            }
            else{
                active_version->snapshot_timestamp = time(0);
                active_version->message = msg;
            }
        }
        bool rollback(const int& v_id = -1){
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
        void history(){
            vector<TreeNode*> snapshots;
            for(auto curr = active_version; curr != nullptr; curr = curr->parent){
                if(curr->snapshot_timestamp != (time_t)0){
                    snapshots.push_back(curr);
                }
            }
            reverse(snapshots.begin(), snapshots.end());
            for(auto const& node: snapshots){
                cout << "Version ID: " << node->version_id << ", Message: " << node->message 
                     << ", Created At: " << ctime(&node->created_timestamp) 
                     << ", Snapshot At: " << ctime(&node->snapshot_timestamp) << endl;
            }
        }
};
class HashMap_File{
    private:
        class HashNode{
            public: 
                string key;
                File* value;
                HashNode* next;
                HashNode(string k, File* v){
                    key = k;
                    value = v;
                    next = nullptr;
                }
        };

        vector<HashNode*> table;
        int curr_size;
        int max_size;

        static constexpr double load_factor = 0.75;

        unsigned long hash_function(const string& key){
            unsigned long hash = 5381;
            for(char c : key){
                hash = ((hash << 5) + hash) + c;
            }
            return hash % max_size;
        }

        void rehash(){  
            int old_max = max_size;
            max_size *= 2;
            vector<HashNode*> new_table(max_size, nullptr);
            rep(i, old_max){
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
    public:
        HashMap_File(int initial_size = 16){
            max_size = initial_size;
            curr_size = 0;
            table.resize(max_size, nullptr);
        }
        ~HashMap_File(){
            for(auto front: table){
                while(front != NULL){
                    HashNode* temp = front;
                    front = front->next;
                    delete temp;
                }
            }
        }
        void insert(const string& key, File* value){
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
        File* get(const string& key){
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
        bool remove(const string& key){
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
};
template<typename T>
class MaxHeap{
    private:
        vector<T> heap;
        void heapify_up(int idx){
            while(idx > 0){
                int par = (idx - 1)/2;
                if(heap[idx] > heap[par]){
                    swap(heap[idx], heap[par]);
                    idx = par;
                }
                else{
                    break;
                }
            }
        }
        void heapify_down(int idx){
            int n = heap.size();
            while(true){
                int left = 2 * idx + 1;
                int right = 2 * idx + 2;
                int largest = idx;
                if(left < n && heap[left] > heap[largest]){
                    largest = left;
                }
                if(right < n && heap[right] > heap[largest]){
                    largest = right;
                }
                if(largest != idx){
                    swap(heap[idx], heap[largest]);
                    idx = largest;
                }
                else{
                    break;
                }
            }
        }
    public:
        bool empty(){
            return heap.empty();
        }
        void insert(const T& value){
            heap.push_back(value);
            heapify_up((int)heap.size() - 1);
        }
        void delete_at(int idx){
            int n = heap.size();
            if(idx < 0 || idx >= n){
                throw out_of_range("Index out of range");
            }
            if(idx == n - 1){
                heap.pop_back();
                return;
            }
            swap(heap[idx], heap.back());
            heap.pop_back();
            if(idx > 0 && heap[idx] > heap[(idx - 1)/2]){
                heapify_up(idx);
            }
            else{ 
                heapify_down(idx);
            }
        }
        void delete_value(const T& value){
            int n = heap.size();
            rep(i, n){
                if(heap[i] == value){
                    delete_at(i);
                    return;
                }
            }
        }
        T top() const{
            if(!heap.empty()){
                return heap[0];
            }
            throw runtime_error("Heap is empty");
        }
        void extract_max(){
            if(heap.empty()){
                throw runtime_error("Heap is empty");
            }
            swap(heap[0], heap.back());
            heap.pop_back();
            if(!heap.empty()){
                heapify_down(0);
            }
        }
};
class FileLastMod{
    public:
        File* file;
        time_t last_modified;

        bool operator>(const FileLastMod& other) const{
            return last_modified > other.last_modified;
        }
};
class FileSize{
    public:
        File* file;
        int total_versions;

        bool operator>(const FileSize& other) const{
            return total_versions > other.total_versions;
        }
};
class FileSystem{
    private:
        MaxHeap<FileLastMod> recent_heap;
        MaxHeap<FileSize> biggest_heap;
    public:
        void updateFile(File* file){
            recent_heap.insert({file, file->active_version->created_timestamp});
            biggest_heap.insert({file, file->total_versions});
        }
        void deleteFileRecent(FileLastMod top){
            recent_heap.delete_value(top);
        }
        void deleteFileBiggest(FileSize top){
            biggest_heap.delete_value(top);
        }
        void RecentFiles(int num){
            vector<FileLastMod> files;
            int count = 0;
            while(!recent_heap.empty() && count < num){
                auto top = recent_heap.top();
                recent_heap.extract_max();
                if(top.file->active_version->created_timestamp == top.last_modified){
                    cout << "File: " << top.file->filename << ", Last Modified: " << ctime(&top.last_modified) << endl;
                    count++;
                    files.push_back(top);
                }
            }
            for(auto const& file : files){
                recent_heap.insert(file);
            }
        }
        void BiggestFiles(int num){
            vector<FileSize> files;
            int count = 0;
            while(!biggest_heap.empty() && count < num){
                auto top = biggest_heap.top();
                biggest_heap.extract_max();
                if(top.file->total_versions == top.total_versions){
                    cout << "File: " << top.file->filename << ", Total Versions: " << top.total_versions << endl;
                    count++;
                    files.push_back(top);
                }
            }
            for(auto const& file : files){
                biggest_heap.insert(file);
            }
        }
};
int main(){ 
    fastio();
    HashMap_File file_map;
    FileSystem file_system;
    cout << "Enter Command ( type 'EXIT' to quit ):" << endl;
    string line;
    while(getline(cin, line)){
        if(line.empty()){
            continue;
        }
        stringstream ss(line);
        string command;
        ss >> command;
        if(command == "EXIT"){
            break;
        }
        else if(command == "CREATE"){
            string filename;
            ss >> filename;
            if(file_map.get(filename) != nullptr){
                cout << "Error: File already exists." << endl;
            }
            else{
                File* new_file = new File(filename, *(new HashMap()));
                file_map.insert(filename, new_file);
                file_system.updateFile(new_file);
                cout << "File " << filename << " created successfully." << endl;
            }
        }
        else if(command == "READ"){
            string filename;
            ss >> filename;
            if(file_map.get(filename) == nullptr){
                cout << "Error: File does not exist." << endl;
            }
            else{
                cout << "Content of file " << filename << ": ";
                File* file = file_map.get(filename);
                file->read();
            }
        }
        else if(command == "INSERT"){
            string filename, content;
            ss >> filename;
            getline(ss, content);
            if(file_map.get(filename) == nullptr){
                cout << "Error: File does not exist." << endl;
            }
            else{
                File* file = file_map.get(filename);
                FileLastMod top_recent = {file, file->active_version->created_timestamp};
                FileSize top_size = {file, file->total_versions};
                file_system.deleteFileRecent(top_recent);
                file_system.deleteFileBiggest(top_size);
                file->insert_content(content.size() > 1 ? content.substr(1) : "");
                file_system.updateFile(file);
                cout << "Content inserted successfully in " << filename << endl;
            }
        }
        else if(command == "UPDATE"){
            string filename, content;
            ss >> filename;
            getline(ss, content);
            if(file_map.get(filename) == nullptr){
                cout << "Error: File does not exist." << endl;
            }
            else{
                File* file = file_map.get(filename);
                FileLastMod top_recent = {file, file->active_version->created_timestamp};
                FileSize top_size = {file, file->total_versions};
                file_system.deleteFileRecent(top_recent);
                file_system.deleteFileBiggest(top_size);
                file->update_content(content.size() > 1 ? content.substr(1) : "");
                file_system.updateFile(file);
                cout << "Content updated successfully in " << filename << endl;
            }
        }
        else if(command == "SNAPSHOT"){
            string filename, message;
            ss >> filename;
            getline(ss, message);
            if(file_map.get(filename) == nullptr){
                cout << "Error: File does not exist." << endl;
            }
            else{
                File* file = file_map.get(filename);
                FileLastMod top_recent = {file, file->active_version->created_timestamp};
                FileSize top_size = {file, file->total_versions};
                file_system.deleteFileRecent(top_recent);
                file_system.deleteFileBiggest(top_size);
                file->snapshot(message.size() > 1 ? message.substr(1) : "");
                file_system.updateFile(file);
                cout << "Snapshot created successfully for " << filename << endl;
            }
        }
        else if(command == "ROLLBACK"){
            string filename;
            ss >> filename;
            if(file_map.get(filename) == nullptr){
                cout << "Error: File does not exist." << endl;
            }
            else{
                string v_id;
                int ver_id;
                if(ss >> v_id){
                    if(!v_id.empty()){
                        try{
                             ver_id = stoi(v_id);
                        }
                        catch(exception& e){
                            cout << "Error: Invalid version ID. Skipping Command" << endl;
                            continue;
                        }
                    }
                    File* file = file_map.get(filename);
                    FileLastMod top_recent = {file, file->active_version->created_timestamp};
                    FileSize top_size = {file, file->total_versions};
                    if(file->rollback(ver_id)){
                        file_system.deleteFileRecent(top_recent);
                        file_system.deleteFileBiggest(top_size);
                        file_system.updateFile(file);
                        cout << "Rolled back to version " << ver_id << " for file " << filename << endl;
                    }
                    else{
                        cout << "Rollback failed for file " << filename << endl;
                    }                
                }
                else{
                    File* file = file_map.get(filename);
                    FileLastMod top_recent = {file, file->active_version->created_timestamp};
                    FileSize top_size = {file, file->total_versions};
                    if(file->rollback()){
                        file_system.deleteFileRecent(top_recent);
                        file_system.deleteFileBiggest(top_size);
                        file_system.updateFile(file);
                        cout << "Rolled back to parent version for file " << filename << endl;
                    }
                    else{
                        cout << "Rollback failed for file " << filename << endl;
                    }  
                }
            }
        }
        else if(command == "HISTORY"){
            string filename;
            ss >> filename;
            if(file_map.get(filename) == nullptr){
                cout << "Error: File does not exist." << endl;
            }
            else{
                File* file = file_map.get(filename);
                cout << "History for file " << filename << ":" << endl;
                file->history();
            }
        }
        else if(command == "RECENT_FILES"){
            string num_str;
            ss >> num_str;
            int num;
            try{
                num = stoi(num_str);
            }
            catch(exception& e){
                cout << "Error: Invalid format of number. Skipping Command" << endl;
                continue;
            }
            cout << "Most recent " << num << " files:" << endl;
            file_system.RecentFiles(num);
        }
        else if(command == "BIGGEST_TREES"){
            string num_str;
            ss >> num_str;
            int num;
            try{
                num = stoi(num_str);
            }
            catch(exception& e){
                cout << "Error: Invalid format of number. Skipping Command." << endl;
                continue;
            }
            cout << "Biggest " << num << " files:" << endl;
            file_system.BiggestFiles(num);
        }
        else{
            cout << "Error: Unknown command." << endl;
        }
    }
    return 0;
}