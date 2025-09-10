#ifndef FILESIZE_HPP
#define FILESIZE_HPP

#include "File.hpp"

class FileSize{
    public:
        File* file;
        int total_versions;

    bool operator>(const FileSize& other) const;
    bool operator<(const FileSize& other) const;
    bool operator==(const FileSize& other) const;
};

#endif