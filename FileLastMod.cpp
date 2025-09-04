#include "FileLastMod.hpp"

bool FileLastMod::operator>(const FileLastMod& other) const {
    return last_modified > other.last_modified;
}

bool FileLastMod::operator==(const FileLastMod& other) const {
    return file == other.file && last_modified == other.last_modified;
}
