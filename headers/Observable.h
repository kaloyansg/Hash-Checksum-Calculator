#pragma once
#include "Observer.h"
#include <vector>
class Observable {
    std::vector<Observer*> observers;
public:
    virtual ~Observable() = default;
    virtual void addObserver(Observer* o);

    void notifyStart(const std::string& filename) const;
    void notifyProgress(size_t bytesProcessed, size_t totalBytes) const;
    void notifyComplete(const std::string& filename, const std::string& checksum) const;
};

