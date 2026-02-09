#include <catch2/catch_test_macros.hpp>
#include <filesystem>
#include <string>

#include "SimpleFileSystemBuilder.h"
#include "SystemItem.h"
#include "Directory.h"
#include "File.h"
#include "Visitor.h" 

namespace fs = std::filesystem;

class SearchVisitor : public Visitor {
public:
    std::string targetName;
    bool found = false;

    SearchVisitor(std::string name) : targetName(name) {}

    void visit(File* file) override {
        if (file->getName() == targetName) {
            found = true;
        }
    }

    void visit(Directory* dir) override {
        if (dir->getName() == targetName) {
            found = true;
        }

        for (const std::unique_ptr<SystemItem>& child : this->getChildren(dir)) {
            child->accept(this);
        }
    }
};


TEST_CASE("SimpleFileSystemBuilder: Tree Construction") {

    fs::path rootPath = fs::path(TEST_DATA_DIR) / "testFiles";
    
    if (!fs::exists(rootPath)) {
        FAIL("Test Resource missing! Could not find: " << rootPath.string());
    }

    SimpleFileSystemBuilder builder;
    std::unique_ptr<SystemItem> root = builder.build(rootPath);

    SECTION("Root is created correctly") {
        REQUIRE(root != nullptr);

        REQUIRE(root->getName() == "testFiles");

        REQUIRE(dynamic_cast<Directory*>(root.get()) != nullptr);
    }

    SECTION("Tree contains expected children files (abc.txt)") {
        SearchVisitor finder("abc.txt");
        root->accept(&finder);

        REQUIRE(finder.found == true);
    }

    SECTION("Tree contains expected sub-directories (music)") {
        SearchVisitor finder("music");
        root->accept(&finder);

        REQUIRE(finder.found == true);
    }
}