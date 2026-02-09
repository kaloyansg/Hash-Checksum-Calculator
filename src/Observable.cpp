#include "Observable.h"

void Observable::addObserver(Observer* o) {
    observers.push_back(o);
}

void Observable::notifyStart(const std::string& filename) const {
    for (Observer* o : observers) o->onStart(filename);
}

void Observable::notifyProgress(size_t bytesProcessed, size_t totalBytes) const {
    for (Observer* o : observers) o->onProgress(bytesProcessed, totalBytes);
}

void Observable::notifyComplete(const std::string& filename, const std::string& checksum) const {
    for (Observer* o : observers) o->onComplete(filename, checksum);
}
