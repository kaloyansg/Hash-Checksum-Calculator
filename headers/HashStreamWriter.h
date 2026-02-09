#pragma once
#include "ProcessingVisitor.h"
#include "ChecksumCalculator.h"
#include "ProgressReporter.h"

#include <memory>
#include <fstream>

class HashStreamWriter : public ProcessingVisitor, public Observable {
private:
    std::unique_ptr<ChecksumCalculator> calculator;
    PauseController* controller = nullptr;//////////////////////////////////////////////
    ProgressReporter* reporter = nullptr;/////////////////////////////////////////

public:
    HashStreamWriter(std::unique_ptr<ChecksumCalculator> calc, 
        std::ostream& outStream = std::cout, std::istream& inStream = std::cin)
        : calculator(std::move(calc)), ProcessingVisitor(outStream,inStream) {};

    void addObserver(Observer* o) override;

    void setController(PauseController* c);

    ScanMemento createMemento();

    void restoreMemento(const ScanMemento& m) {
        if (reporter) {
            reporter->restoreFromMemento(m);
        }
        std::cout << "[System] State Restored. Resuming from: " << m.elapsedSeconds << "s elapsed." << std::endl;
    }

protected:
    void processFile(File* file) override;
    void processDir(Directory* dir) override;
};

class HashStreamBuilder : public VisitorBuilder {
public:
    HashStreamBuilder();

    virtual std::string getFLag() override;
    virtual void action(int argc, char* argv[], std::unique_ptr<ChecksumCalculator>& calc,
        std::unique_ptr<SystemItem>& root) override;
    virtual std::unique_ptr<VisitorBuilder> clone() const override;

private:
    const std::string BLUE = "\033[36m";
    const std::string RESET = "\033[0m";

    static void scanTask(SystemItem* root, HashStreamWriter* writer);
};