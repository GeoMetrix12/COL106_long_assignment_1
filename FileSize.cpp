#include "FileSize.hpp"

bool FileSize::operator>(const FileSize& other) const {
    if (total_versions > other.total_versions) {
        return true;
    } else if (total_versions < other.total_versions) {
        return false;
    } else {
        return file->last_modified_time > other.file->last_modified_time;
    }
}

bool FileSize::operator<(const FileSize& other) const {
    if (total_versions < other.total_versions) {
        return true;
    } else if (total_versions > other.total_versions) {
        return false;
    } else {
        return file->last_modified_time < other.file->last_modified_time;
    }
}

bool FileSize::operator==(const FileSize& other) const {
    return file == other.file && total_versions == other.total_versions;
}
