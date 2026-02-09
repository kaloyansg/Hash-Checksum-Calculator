#pragma once
#include "FileSystemBuilder.h"
#include "File.h"
#include "Directory.h"
#include <set>
#include <iostream>

class SmartFileSystemBuilder : public FileSystemBuilder {
private:
    std::set<std::filesystem::path> visitedPaths; //to find cycles

public:
    std::unique_ptr<SystemItem> build(const std::filesystem::path& path) override;
};