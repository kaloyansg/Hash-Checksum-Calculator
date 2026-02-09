#include <catch2/catch_test_macros.hpp>
#include <iostream>
#include <sstream>
#include <thread>
#include <chrono>

#include "ProgressReporter.h"

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

    void clear() {
        buffer.str("");
        buffer.clear();
    }
};


TEST_CASE("ProgressReporter: Output Formatting") {
    size_t totalBytes = 1000;
    ProgressReporter reporter(totalBytes);
    CoutRedirect capture;

    SECTION("Start Message") {
        reporter.onStart("test_file.txt");

        std::string output = capture.str();
        REQUIRE(output.find("Processing test_file.txt") != std::string::npos);
    }

    SECTION("Progress Calculation (Percentage)") {
        reporter.onStart("file1.txt");
        capture.clear();

        reporter.onProgress(500, 500);

        std::string output = capture.str();

        REQUIRE(output.find("[Working]") != std::string::npos);
        REQUIRE(output.find("(Total: 50%)") != std::string::npos);
        REQUIRE(output.find("Processing: 500 bytes") != std::string::npos);
    }

    SECTION("Completion Message") {
        reporter.onComplete("file1.txt", "HASH_123");

        std::string output = capture.str();
        REQUIRE(output.find("[Done] file1.txt -> HASH_123") != std::string::npos);
    }
}

TEST_CASE("ProgressReporter: Memento (Save/Restore State)") {
    size_t totalBytes = 2000;
    ScanMemento memento;

    {
        ProgressReporter reporterA(totalBytes);
        reporterA.onStart("fileA.bin");

        reporterA.onProgress(500, 500);

        std::this_thread::sleep_for(std::chrono::milliseconds(50));

        reporterA.saveToMemento(memento);

        REQUIRE(memento.globalBytesProcessed == 500);
        REQUIRE(memento.elapsedSeconds > 0.0);
    }

    {
        ProgressReporter reporterB(totalBytes);
        CoutRedirect capture;

        reporterB.restoreFromMemento(memento);

        reporterB.onStart("fileB.bin");

        reporterB.onProgress(500, 500);

        std::string output = capture.str();

        REQUIRE(output.find("(Total: 50%)") != std::string::npos);

    }
}

TEST_CASE("ProgressReporter: ETA Formatting") {

    ProgressReporter reporter(100000);
    CoutRedirect capture;
    reporter.onStart("large_file.bin");

    std::this_thread::sleep_for(std::chrono::milliseconds(150));
    reporter.onProgress(1000, 1000);

    std::string output = capture.str();

    REQUIRE(output.find("[ETA:") != std::string::npos);
}