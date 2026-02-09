#pragma once
#include <memory>
#include <filesystem>
#include "SystemItem.h"

class Directory;

class FileSystemBuilder {
public:
    virtual ~FileSystemBuilder() = default;
    virtual std::unique_ptr<SystemItem> build(const std::filesystem::path& path) = 0;

protected:
    void addChildToDir(std::unique_ptr<Directory>&, std::unique_ptr<SystemItem> item);
};

