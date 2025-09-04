#ifndef FILELASTMOD_HPP
#define FILELASTMOD_HPP

#include "File.hpp"
#include <ctime>

class FileLastMod{
    public:
        File* file;
        time_t last_modified;

        bool operator>(const FileLastMod& other) const;
        bool operator==(const FileLastMod& other) const;
};

#endif
