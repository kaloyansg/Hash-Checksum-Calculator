#include <catch2/catch_test_macros.hpp>
#include <sstream>
#include <iostream>
#include <filesystem>

#include "PrintVisitor.h"
#include "File.h"
#include "Directory.h"

struct CoutRedirector {
    std::streambuf* old;
    std::stringstream buffer;

    CoutRedirector() {
        old = std::cout.rdbuf(buffer.rdbuf());
    }

    ~CoutRedirector() {
        std::cout.rdbuf(old);
    }

    std::string getString() {
        return buffer.str();
    }
};

TEST_CASE("PrintVisitor: Output Formatting") {
    std::filesystem::path resourceBase(TEST_DATA_DIR);
    std::filesystem::path abcPath = resourceBase / "testFiles" / "abc.txt";

    if (!std::filesystem::exists(abcPath)) {
        FAIL("Setup Error: 'abc.txt' not found at " << abcPath.string());
    }

    uintmax_t expectedSize = std::filesystem::file_size(abcPath);

    PrintVisitor visitor;

    SECTION("Visits File and prints Name + Size") {
        CoutRedirector capture;

        File fileObj("abc.txt", abcPath);

        fileObj.accept(&visitor);

        std::string output = capture.getString();

        REQUIRE(output.find("- [File] abc.txt") != std::string::npos);

        std::string sizeString = "(" + std::to_string(expectedSize) + " bytes)";
        REQUIRE(output.find(sizeString) != std::string::npos);
    }

    SECTION("Visits Directory and prints Name") {
        CoutRedirector capture;

        Directory dirObj("MyPhotos", resourceBase / "testFiles" / "photos");

        dirObj.accept(&visitor);

        std::string output = capture.getString();

        REQUIRE(output.find("[MyPhotos]") != std::string::npos);
    }
}

TEST_CASE("PrinterBuilder: Configuration") {
    PrinterBuilder builder;

    SECTION("Registers correct flag") {
        REQUIRE(builder.getFLag() == "--print");
    }

    SECTION("Clone creates a new instance") {
        auto clone = builder.clone();
        REQUIRE(clone != nullptr);

        auto* casted = dynamic_cast<PrinterBuilder*>(clone.get());

        REQUIRE(casted != nullptr);
        REQUIRE(casted->getFLag() == "--print");
    }
}