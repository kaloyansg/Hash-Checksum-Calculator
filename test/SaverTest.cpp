#include <catch2/catch_test_macros.hpp>
#include <filesystem>
#include <fstream>
#include <string>
#include <sstream>

#include "Saver.h"
#include "ChecksumCalculator.h"
#include "File.h"

namespace fs = std::filesystem;

namespace {
    class MockSaverCalculator : public ChecksumCalculator {
    public:
        std::string calculate(std::istream&) const override {
            return "FIXED_HASH_123";
        }
    };
}

static std::string readFileContent(const fs::path& path) {
    std::ifstream f(path);
    std::stringstream buffer;
    buffer << f.rdbuf();
    return buffer.str();
}

TEST_CASE("Saver: File Operations") {
    // Setup Paths
    fs::path inputPath = fs::path(TEST_DATA_DIR) / "testFiles" / "abc.txt";
    fs::path outputPath = "temp_checksum_output.txt";

    if (fs::exists(outputPath)) fs::remove(outputPath);

    SECTION("Writes checksum correctly for a valid file") {
        {
            // MockSaverCalculator
            std::unique_ptr<ChecksumCalculator> calc = std::make_unique<MockSaverCalculator>();
            Saver saver(std::move(calc), outputPath);

            File fileObj("abc.txt", inputPath);
            saver.visit(&fileObj);
        }

        std::string content = readFileContent(outputPath);

        REQUIRE(content.find("abc.txt") != std::string::npos);
        REQUIRE(content.find("FIXED_HASH_123") != std::string::npos);
    }

    if (fs::exists(outputPath)) fs::remove(outputPath);
}

TEST_CASE("SaverBuilder: Validation") {
    SaverBuilder builder;

    std::unique_ptr<ChecksumCalculator> calc = std::unique_ptr<ChecksumCalculator>(new MockSaverCalculator());

    fs::path realPath = fs::path(TEST_DATA_DIR) / "testFiles" / "abc.txt";
    std::unique_ptr<File> rootRaw = std::make_unique<File>("rootFile", realPath);

    std::unique_ptr<SystemItem> root = std::move(rootRaw);

    SECTION("Throws if output file argument is missing") {
        char* argv[] = { (char*)"exe", (char*)"target", (char*)"--save" };
        int argc = 3;

        REQUIRE_THROWS_AS(builder.action(argc, argv, calc, root), std::exception);
    }

    SECTION("Succeeds with correct arguments") {
        char* argv[] = { (char*)"exe", (char*)"target", (char*)"--save", (char*)"temp_builder_out.txt" };
        int argc = 4;

        REQUIRE_NOTHROW(builder.action(argc, argv, calc, root));

        if (fs::exists("temp_builder_out.txt")) fs::remove("temp_builder_out.txt");
    }
}