#include <iostream>
#include <string>
#include <sstream>
#include "FileSystem.hpp"
#include "HashMap_File.hpp"
#include "File.hpp"
#define fastio() ios::sync_with_stdio(false); cin.tie(NULL); cout.tie(NULL);
using namespace std;

int main(){ 
    fastio();
    HashMap_File file_map;
    FileSystem file_system;
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