#include <catch2/catch_test_macros.hpp>
#include <sstream>
#include <filesystem> 

#include "HashStreamWriter.h"
#include "ChecksumCalculator.h"
#include "File.h"       
#include "Directory.h"  


class MockHashCalculator : public ChecksumCalculator {
public:
    std::string calculate(std::istream&) const override {
        return "MOCK_HASH_123";
    }
};

class TestableHashStreamWriter : public HashStreamWriter {
public:
    TestableHashStreamWriter(std::unique_ptr<ChecksumCalculator> calc,
        std::ostream& out, std::istream& in)
        : HashStreamWriter(std::move(calc), out, in) {
    }

    void testProcessFile(File* f) { this->processFile(f); }
    void testProcessDir(Directory* d) { this->processDir(d); }
};


TEST_CASE("HashStreamWriter: Output Redirection with Real Files") {

    // spy
    std::stringstream mockOutput;
    std::stringstream mockInput;
    auto calc = std::make_unique<MockHashCalculator>();

    TestableHashStreamWriter writer(std::move(calc), mockOutput, mockInput);

    // resources
    std::filesystem::path resourceBase(TEST_DATA_DIR);
    std::filesystem::path testFilesDir = resourceBase / "testFiles";

    SECTION("Writes correct format for 'abc.txt'") {
        std::filesystem::path filePath = testFilesDir / "abc.txt";

        if (!std::filesystem::exists(filePath)) {
            FAIL("Test file not found at: " << filePath.string());
        }

        File fileObj("abc.txt", filePath);

        REQUIRE_NOTHROW(writer.testProcessFile(&fileObj));

        std::string actualOutput = mockOutput.str();
        REQUIRE(actualOutput.find("- abc.txt") != std::string::npos);
    }

    SECTION("Writes correct format for 'music' Directory") {
        std::filesystem::path dirPath = testFilesDir / "music";

        Directory dirObj("music", dirPath);

        writer.testProcessDir(&dirObj);

        std::string actualOutput = mockOutput.str();
        REQUIRE(actualOutput.find("[music]") != std::string::npos);
    }
}