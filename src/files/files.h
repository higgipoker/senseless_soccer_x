#pragma once
#include <filesystem>
#include <vector>
#include <string>
#include <fstream>
namespace fs = std::filesystem;
namespace ss {
namespace files {

struct File {
    std::string path;
    std::fstream fs;
    std::vector<std::string> lines;
};

std::string working_dir();
void open (File& file, const std::string& path, const std::ios_base::openmode mode = std::fstream::in | std::fstream::out | std::fstream::app);
void close (File& file);
void read_lines (File& file);
void write_line (File& file, const std::string& line);
}
}
