#include "FileLastMod.hpp"

bool FileLastMod::operator>(const FileLastMod& other) const {
    if (last_modified > other.last_modified) {
        return true;
    } else if (last_modified < other.last_modified) {
        return false;
    } else {
        // Tiebreaker: last_execution_number
        return file->last_execution_number > other.file->last_execution_number;
    }
}

bool FileLastMod::operator<(const FileLastMod& other) const {
    if (last_modified < other.last_modified) {
        return true;
    } else if (last_modified > other.last_modified) {
        return false;
    } else {
        // Tiebreaker: last_execution_number
        return file->last_execution_number < other.file->last_execution_number;
    }
}

bool FileLastMod::operator==(const FileLastMod& other) const {
    return file == other.file && last_modified == other.last_modified;
}
