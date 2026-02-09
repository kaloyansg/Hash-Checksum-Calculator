#pragma once
#include "Visitor.h"
#include "File.h"
#include "Directory.h"
#include <iostream>

class ProcessingVisitor : public Visitor {
protected:
    int indentLevel = 0;

    std::string getIndent() const;

    virtual void processFile(File* file) = 0;
    virtual void processDir(Directory* dir);

public:
    ProcessingVisitor(std::ostream& outStream = std::cout, std::istream& inStream = std::cin)
        : Visitor(outStream,inStream) {}
    void visit(Directory* dir) override;
    void visit(File* file) override;
};