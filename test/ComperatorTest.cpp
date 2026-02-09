#include <catch2/catch_test_macros.hpp>
#include <catch2/matchers/catch_matchers_all.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <vector>

#include "Comperator.h"
#include "ChecksumCalculator.h"
#include "File.h"

namespace fs = std::filesystem;

namespace {
    class MockCompCalculator : public ChecksumCalculator {
    public:
        std::string calculate(std::istream&) const override {
            return "HASH_123";
        }
    };

    struct CoutRedirect {
        std::stringstream buffer;
        std::streambuf* old;
        CoutRedirect() { old = std::cout.rdbuf(buffer.rdbuf()); }
        ~CoutRedirect() { std::cout.rdbuf(old); }
        std::string str() { return buffer.str(); }
    };
}

TEST_CASE("Comperator: Full Integration") {

    fs::path tempDir = "temp_comperator_test";

    if (fs::exists(tempDir)) fs::remove_all(tempDir);
    fs::create_directory(tempDir);

    fs::path pOK = tempDir / "ok.txt";
    fs::path pMod = tempDir / "mod.txt";
    fs::path pNew = tempDir / "new.txt";
    fs::path pRem = tempDir / "removed.txt"; 

    { std::ofstream(pOK) << "content"; }
    { std::ofstream(pMod) << "content"; }
    { std::ofstream(pNew) << "content"; }

    fs::path snapshotPath = tempDir / "snapshot.txt";
    {
        std::ofstream snap(snapshotPath);

        snap << "\"" << pOK.string() << "\" HASH_123" << std::endl;

        snap << "\"" << pMod.string() << "\" OLD_HASH_999" << std::endl;

        snap << "\"" << pRem.string() << "\" HASH_123" << std::endl;
    }

    CoutRedirect capture;

    {
        auto calc = std::make_unique<MockCompCalculator>();

        Comperator visitor(std::move(calc), snapshotPath);

        File fOK("ok.txt", pOK);
        visitor.visit(&fOK);

        File fMod("mod.txt", pMod);
        visitor.visit(&fMod);

        File fNew("new.txt", pNew);
        visitor.visit(&fNew);

    }

    std::string output = capture.str();

    REQUIRE(output.find(pOK.string() + ": \033[32mOK") != std::string::npos);

    REQUIRE(output.find(pMod.string() + ": \033[33mMODIFIED") != std::string::npos);

    REQUIRE(output.find(pNew.string() + ": \033[33mNEW") != std::string::npos);

    REQUIRE(output.find(pRem.string() + ": \033[31mREMOVED") != std::string::npos);

    if (fs::exists(tempDir)) fs::remove_all(tempDir);
}

TEST_CASE("ComperatorBuilder: Validation") {
    ComperatorBuilder builder;
    auto calc = std::unique_ptr<ChecksumCalculator>(new MockCompCalculator());

    fs::path tempRoot = "temp_dummy_root.txt";
    { std::ofstream t(tempRoot); }

    auto rootRaw = std::make_unique<File>("root", tempRoot);
    std::unique_ptr<SystemItem> root = std::move(rootRaw);

    SECTION("Flag Check") {
        REQUIRE(builder.getFLag() == "--checksums");
    }

    SECTION("Throws if snapshot argument is missing") {
        char* argv[] = { (char*)"app", (char*)"path", (char*)"--checksums" };
        REQUIRE_THROWS_WITH(
            builder.action(3, argv, calc, root),
            Catch::Matchers::ContainsSubstring("Must point a source file")
        );
    }

    fs::remove(tempRoot);
}