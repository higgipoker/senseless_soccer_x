#include <filesystem>
#include <string>
#include <vector>
#include <set>

namespace fs = std::filesystem;
namespace ss {
namespace files {

static std::string working_dir() {
    return std::filesystem::current_path().string();
}

static std::set<std::string> get_file_list (const std::string& path) {
    std::set<std::string> files;
    for (const auto& entry : fs::directory_iterator (path)) {
        files.insert (entry.path().string());
    }
    return files;
}

}
}
