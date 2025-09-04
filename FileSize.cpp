#include "FileSize.hpp"

bool FileSize::operator>(const FileSize& other) const {
    return total_versions > other.total_versions;
}

bool FileSize::operator==(const FileSize& other) const {
    return file == other.file && total_versions == other.total_versions;
}
