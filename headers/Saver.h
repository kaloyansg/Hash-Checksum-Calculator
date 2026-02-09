#pragma once
#include "Visitor.h"
#include "ChecksumCalculator.h"
#include <fstream>

class Saver : public Visitor
{
private:
	std::unique_ptr<ChecksumCalculator> calculator;
    std::ofstream writer;

public:
    Saver(std::unique_ptr<ChecksumCalculator> calc, const std::filesystem::path& path);
    ~Saver();

    virtual void visit(File* file) override;
    virtual void visit(Directory* directory) override; //const

private:
    const std::string RED = "\033[31m";
    const std::string RESET = "\033[0m";
};

class SaverBuilder : public VisitorBuilder {
public:
    SaverBuilder();

    virtual std::string getFLag() override;
    virtual void action(int argc, char* argv[], std::unique_ptr<ChecksumCalculator>& calc,
        std::unique_ptr<SystemItem>& root) override;
    virtual std::unique_ptr<VisitorBuilder> clone() const override;

private:
    const std::string GREEN = "\033[32m";
    const std::string RESET = "\033[0m";
};