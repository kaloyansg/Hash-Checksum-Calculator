#pragma once
#include "Visitor.h"
#include "File.h"
#include "Directory.h"
#include <iostream>
#include <string>

class PrintVisitor : public Visitor {
private:
    int indentLevel = 0;

    std::string getIndent() const;

public:
    void visit(File* file) override;
    void visit(Directory* dir) override;
};

class PrinterBuilder : public VisitorBuilder {
public:
    PrinterBuilder();

    virtual std::string getFLag() override;
    virtual void action(int argc, char* argv[], std::unique_ptr<ChecksumCalculator>& calc,
        std::unique_ptr<SystemItem>& root) override;
    virtual std::unique_ptr<VisitorBuilder> clone() const override;
};