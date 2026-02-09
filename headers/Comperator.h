#pragma once
#include "Visitor.h"
#include "ChecksumCalculator.h"
#include <vector>
#include <fstream>

class Comperator : public Visitor
{
private:
    struct SnapshotEntry {
        std::string path;
        std::string checksum;
        bool visited;
    };

    std::unique_ptr<ChecksumCalculator> calculator;
    std::vector<SnapshotEntry> content;
    
public:
    Comperator(std::unique_ptr<ChecksumCalculator> calc, const std::filesystem::path& path);
    ~Comperator();

    virtual void visit(File* file) override;
    virtual void visit(Directory* directory) override; //const

private:
    const std::string RED = "\033[31m";
    const std::string GREEN = "\033[32m";
    const std::string YELLOW = "\033[33m";
    const std::string RESET = "\033[0m";
};

class ComperatorBuilder : public VisitorBuilder {
public:
    ComperatorBuilder();

    virtual std::string getFLag() override;
    virtual void action(int argc, char* argv[], std::unique_ptr<ChecksumCalculator>& calc,
        std::unique_ptr<SystemItem>& root) override;
    virtual std::unique_ptr<VisitorBuilder> clone() const override;
};