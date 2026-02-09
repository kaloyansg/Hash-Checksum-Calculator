#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_all.hpp>
#include <iostream>
#include <sstream>
#include <filesystem>
#include <memory>
#include <fstream>

#include "XML_RW.h"
#include "ChecksumCalculator.h"
#include "File.h"
#include "Directory.h"

namespace fs = std::filesystem;

namespace {

    class MockXMLCalculator : public ChecksumCalculator {
    public:
        std::string calculate(std::istream&) const override {
            return "XML_MOCK_HASH";
        }
    };

    struct CoutRedirect {
        std::stringstream buffer;
        std::streambuf* old;

        CoutRedirect() {
            old = std::cout.rdbuf(buffer.rdbuf());
        }

        ~CoutRedirect() {
            std::cout.rdbuf(old);
        }

        std::string str() { return buffer.str(); }
    };

    class TestableXML_RW : public XML_RW {
    public:
        using XML_RW::XML_RW;

        void testProcessFile(File* f) {
            this->processFile(f);
        }

        void testProcessDir(Directory* d) {
            this->processDir(d);
        }
    };

}

TEST_CASE("XML_RW: XML Output Generation") {

    fs::path realPath = fs::path(TEST_DATA_DIR) / "testFiles" / "abc.txt";

    std::unique_ptr<ChecksumCalculator> calc = std::make_unique<MockXMLCalculator>();
    TestableXML_RW writer(std::move(calc));

    SECTION("Generates correct XML for a single file") {
        CoutRedirect capture;

        File fileObj("abc.txt", realPath);

        writer.testProcessFile(&fileObj);

        std::string output = capture.str();

        REQUIRE(output.find("<item>") != std::string::npos);
        REQUIRE(output.find("<mode>binary</mode>") != std::string::npos);
        REQUIRE(output.find("<checksum>XML_MOCK_HASH</checksum>") != std::string::npos);

        std::string expectedPath = "<path>" + realPath.string() + "</path>";
        REQUIRE(output.find(expectedPath) != std::string::npos);

        REQUIRE(output.find("</item>") != std::string::npos);
    }

    SECTION("Generates correct XML for a directory") {
        CoutRedirect capture;

        Directory dirObj("MyFolder", fs::path("dummy/path"));

        writer.testProcessDir(&dirObj);

        std::string output = capture.str();

        REQUIRE(output.find("<checksum>") != std::string::npos);
        REQUIRE(output.find("</checksum>") != std::string::npos);
    }
}

TEST_CASE("XML_RW: Error Handling") {
    std::unique_ptr<ChecksumCalculator> calc = std::make_unique<MockXMLCalculator>();
    TestableXML_RW writer(std::move(calc));

    SECTION("Throws exception when file cannot be opened") {
        fs::path tempPath = "temp_ghost.txt";
        { std::ofstream t(tempPath); }

        File badFile("ghost.txt", tempPath);

        fs::remove(tempPath);

        REQUIRE_THROWS_WITH(writer.testProcessFile(&badFile), "File could not be opened");
    }
}