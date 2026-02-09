#include "ProgressReporter.h"

ProgressReporter::ProgressReporter(size_t totalBytes)
    : totalGlobalBytes(totalBytes),
    globalBytesProcessed(0),
    currentFileProcessed(0), storedDuration(0) {
    startTime = std::chrono::steady_clock::now();
}

void ProgressReporter::saveToMemento(ScanMemento& memento) {
    std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
    double currentSession = std::chrono::duration<double>(now - startTime).count();

    memento.globalBytesProcessed = globalBytesProcessed;
    memento.elapsedSeconds = storedDuration + currentSession;
}

void ProgressReporter::restoreFromMemento(const ScanMemento& memento) {
    globalBytesProcessed = memento.globalBytesProcessed;
    storedDuration = memento.elapsedSeconds;
    startTime = std::chrono::steady_clock::now();
}

void ProgressReporter::onStart(const std::string& filename) {
    currentFileProcessed = 0;
    std::cout << YELLOW << "Processing " << filename << "... " << RESET << std::endl;
    std::cout.flush();
}

void ProgressReporter::onProgress(size_t bytesProcessedForFile, size_t totalBytes) {
    size_t delta = bytesProcessedForFile - currentFileProcessed;
    currentFileProcessed = bytesProcessedForFile;
    globalBytesProcessed += delta;

    std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
    double currentSession = std::chrono::duration<double>(now - startTime).count();
    double totalElapsed = storedDuration + currentSession;

    std::string etaString = "Calc...";
    if (totalElapsed > 0.1 && globalBytesProcessed > 0) {
        double speed = globalBytesProcessed / totalElapsed;
        double remainingBytes = (double)(totalGlobalBytes - globalBytesProcessed);
        etaString = formatTime(remainingBytes / speed);
    }

    int percent = (totalGlobalBytes > 0) ? (int)((double)globalBytesProcessed / totalGlobalBytes * 100) : 0;

    std::cout << "\r" << YELLOW << "[Working] " << RESET
        << "(Total: " << percent << "%) "
        << "[ETA: " << etaString << "] "
        << "Processing: " << bytesProcessedForFile << " bytes" << std::string(15, ' ') /*<< std::endl*/;

    std::cout.flush();
}

void ProgressReporter::onComplete(const std::string& filename, const std::string& checksum) {
    std::cout << "\r" << std::string(80, ' ') << "\r";
    std::cout << GREEN << "[Done] " << filename << " -> " << checksum << RESET << std::endl;
}

std::string ProgressReporter::formatTime(double seconds) {
    if (seconds < 60) {
        return std::to_string(static_cast<int>(seconds)) + "s";
    }
    else {
        int m = static_cast<int>(seconds) / 60;
        int s = static_cast<int>(seconds) % 60;
        return std::to_string(m) + "m " + std::to_string(s) + "s";
    }
}
