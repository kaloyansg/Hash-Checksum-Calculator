#pragma once
#include "FileSystemBuilder.h"
#include "File.h"
#include "Directory.h"

class SimpleFileSystemBuilder : public FileSystemBuilder { //slink as a file
public:
    std::unique_ptr<SystemItem> build(const std::filesystem::path& path) override;
};

