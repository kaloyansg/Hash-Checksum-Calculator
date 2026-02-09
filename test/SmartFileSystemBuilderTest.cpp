#include <catch2/catch_test_macros.hpp>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>

#include "SmartFileSystemBuilder.h"
#include "SystemItem.h"
#include "Directory.h"
#include "File.h"
#include "Visitor.h" 

namespace fs = std::filesystem;

class FindFileVisitor : public Visitor {
public:
    std::string targetName;
    bool found = false;

    FindFileVisitor(std::string name) : targetName(name) {}

    void visit(File* file) override {
        if (file->getName() == targetName) found = true;
    }

    void visit(Directory* dir) override {
        if (dir->getName() == targetName) found = true;

        for (const std::unique_ptr<SystemItem>& child : this->getChildren(dir)) {
            child->accept(this);
        }
    }
};

struct CoutRedirect {
    std::stringstream buffer;
    std::streambuf* old;
    CoutRedirect() { old = std::cout.rdbuf(buffer.rdbuf()); }
    ~CoutRedirect() { std::cout.rdbuf(old); }
    std::string str() { return buffer.str(); }
};


TEST_CASE("SmartFileSystemBuilder: Symlink Logic") {
    // A. Setup Paths
    fs::path baseDir = fs::path(TEST_DATA_DIR) / "testFiles"; 
    fs::path targetDir = baseDir / "music";
    fs::path linkPath = baseDir / "smart_link_to_music";

    bool symlinkCreated = false;
    try {
        if (!fs::exists(linkPath)) {
            fs::create_directory_symlink(targetDir, linkPath);
        }
        symlinkCreated = true;
    }
    catch (const fs::filesystem_error& e) {
        WARN("Skipping Symlink Test (Permission Denied): " << e.what());
    }

    if (symlinkCreated) {
        SmartFileSystemBuilder builder;

        SECTION("Follows directory symlinks recursively") {
            std::unique_ptr<SystemItem> root = builder.build(linkPath);

            REQUIRE(root != nullptr);

            REQUIRE(dynamic_cast<Directory*>(root.get()) != nullptr);

        }

        fs::remove(linkPath);
    }
}

TEST_CASE("SmartFileSystemBuilder: Cycle Detection") {
    fs::path cycleRoot = fs::path(TEST_DATA_DIR) / "cycleTest";
    fs::path folderA = cycleRoot / "FolderA";
    fs::path folderB = folderA / "FolderB";
    fs::path linkToA = folderB / "LinkBackToA";

    fs::remove_all(cycleRoot);

    fs::create_directories(folderB);

    bool setupSuccess = false;
    try {
        fs::create_directory_symlink(folderA, linkToA);
        setupSuccess = true;
    }
    catch (const std::exception& e) {
        WARN("Skipping Cycle Test (Permission Denied): " << e.what());
    }

    if (setupSuccess) {
        SmartFileSystemBuilder builder;
        CoutRedirect capture;

        std::unique_ptr<SystemItem> root = builder.build(folderA);

        std::string output = capture.str();

        SECTION("Detects cycle and prints warning") {
            REQUIRE(output.find("Cycle detected at") != std::string::npos);
            REQUIRE(output.find("Skipping") != std::string::npos);
        }

        SECTION("Returns a valid tree despite cycle") {
            REQUIRE(root != nullptr);
            REQUIRE(root->getName() == "FolderA");
        }

        fs::remove_all(cycleRoot);
    }
}