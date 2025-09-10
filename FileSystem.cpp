#include "FileSystem.hpp"
#include <iostream>

void FileSystem::updateFile(File* file){
    recent_heap.insert({file, file->last_modified_time});
    biggest_heap.insert({file, file->total_versions});
}
void FileSystem::deleteFileRecent(FileLastMod top){
    recent_heap.delete_value(top.file->filename);
}
void FileSystem::deleteFileBiggest(FileSize top){
    biggest_heap.delete_value(top.file->filename);
}
void FileSystem::RecentFiles(int num){
    vector<FileLastMod> files;
    int count = 0;
    while(!recent_heap.empty() && count < num){
        auto top = recent_heap.top();
        recent_heap.extract_max();
        if(top.file->last_modified_time == top.last_modified){
            cout << "File: " << top.file->filename << ", Last Modified: " << ctime(&top.last_modified);
            count++;
            files.push_back(top);
        }
    }
    if(count < num){
        cout << "Only " << count << " files available." << endl;
    }
    for(auto const& file : files){
        recent_heap.insert(file);
    }
}
void FileSystem::BiggestFiles(int num){
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