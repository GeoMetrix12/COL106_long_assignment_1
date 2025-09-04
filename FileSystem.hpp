#ifndef FILESYSTEM_HPP
#define FILESYSTEM_HPP

#include "MaxHeap.hpp"
#include "FileLastMod.hpp"
#include "FileSize.hpp"
#include "File.hpp"

class FileSystem{
    private:
        MaxHeap<FileLastMod> recent_heap;
        MaxHeap<FileSize> biggest_heap;
    public:
        void updateFile(File* file);
        void deleteFileRecent(FileLastMod top);
        void deleteFileBiggest(FileSize top);
        void RecentFiles(int num);
        void BiggestFiles(int num);
};

#endif