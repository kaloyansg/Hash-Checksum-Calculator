#pragma once
#include <string>
class Observer {
public:
    virtual ~Observer() = default;
    virtual void onStart(const std::string& filename) {}
    virtual void onProgress(size_t bytesProcessed, size_t totalBytes) {}
    virtual void onComplete(const std::string& filename, const std::string& checksum) {}
};

