#include "files.h"
#include <set>
#include <iostream>
#include <cassert>


namespace ss {
namespace files {


std::string working_dir() {
    return std::filesystem::current_path().string();
}

static std::set<std::string> get_file_list (const std::string& path) {
    std::set<std::string> files;
    for (const auto& entry : fs::directory_iterator (path)) {
        files.insert (entry.path().string());
    }
    return files;
}

void open (File& file, const std::string& path, const std::ios_base::openmode mode) {
    if (file.fs.is_open()) {
        file.fs.close();
    }
    file.path = path;
    file.fs.open (path, mode);
    if(file.fs.is_open()){
        read_lines(file);
    }
}

void close (File& file) {
    file.fs.close();
}

void write_line (File& file, const std::string& line) {
    assert (file.fs.is_open());
    file.fs << line << std::endl;
}

void read_lines (File& file) {
    assert (file.fs.is_open());
    file.fs.clear();
    file.fs.seekg (0);
    std::string line;
    file.lines.clear();
    while (getline (file.fs, line)) {
        file.lines.push_back (line);
    }
}

}// namespace
}// namespace
