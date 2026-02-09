#include "SimpleFileSystemBuilder.h"

std::unique_ptr<SystemItem> SimpleFileSystemBuilder::build(const std::filesystem::path& path) {
    if (std::filesystem::is_directory(path) && !std::filesystem::is_symlink(path)) {
        std::unique_ptr<Directory> dir = std::make_unique<Directory>(path.filename().string(), path);

        for (const std::filesystem::directory_entry& entry : std::filesystem::directory_iterator(path)) {
            this->addChildToDir(dir, build(entry.path()));
            //dir->add(build(entry.path()));
        }
        return dir;
    }
    else {
        return std::make_unique<File>(path.filename().string(), path);
    }
}
