#pragma once
#include "Observer.h"
#include <iostream>
#include <iomanip>
#include <chrono>

struct ScanMemento {
    // ProgressReporter state
    size_t globalBytesProcessed = 0;
    double elapsedSeconds = 0;

    // HashStreamWriter state
    std::string currentFilePath;
};

class ProgressReporter : public Observer {
public:
    ProgressReporter(size_t totalBytes);

    void saveToMemento(ScanMemento& memento);

    void restoreFromMemento(const ScanMemento& memento);

    void onStart(const std::string& filename) override;

    void onProgress(size_t bytesProcessed, size_t totalBytes) override;

    void onComplete(const std::string& filename, const std::string& checksum) override;

private:
    std::string formatTime(double seconds);

private:
    size_t totalGlobalBytes;
    size_t globalBytesProcessed;
    size_t currentFileProcessed;

    // for time
    double storedDuration;
    std::chrono::time_point<std::chrono::steady_clock> startTime;

    const std::string GREEN = "\033[32m";
    const std::string YELLOW = "\033[33m";
    const std::string RESET = "\033[0m";
};

