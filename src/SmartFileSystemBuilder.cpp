#include "SmartFileSystemBuilder.h"

std::unique_ptr<SystemItem> SmartFileSystemBuilder::build(const std::filesystem::path& path) {

    std::filesystem::path targetPath = path;

    if (std::filesystem::is_symlink(path)) {
        try {
            targetPath = std::filesystem::canonical(path);
        }
        catch (const std::exception&) {
            return std::make_unique<File>(path.filename().string(), path);
        }
    }

    if (visitedPaths.find(targetPath) != visitedPaths.end()) {
        std::cout << "Cycle detected at: " << path << ". Skipping." << std::endl;
        return nullptr;
    }

    visitedPaths.insert(targetPath);

    std::unique_ptr<SystemItem> result;

    if (std::filesystem::is_directory(targetPath)) {
        std::unique_ptr<Directory> dir = std::make_unique<Directory>(path.filename().string(), targetPath);

        for (const std::filesystem::directory_entry & entry : std::filesystem::directory_iterator(targetPath)) {
            std::unique_ptr<SystemItem> child = build(entry.path());
            if (child) {
                this->addChildToDir(dir, std::move(child));
                //dir->add(std::move(child));
            }
        }
        result = std::move(dir);
    }
    else {
        result = std::make_unique<File>(path.filename().string(), targetPath);
    }
    visitedPaths.erase(targetPath);

    return result;
}
