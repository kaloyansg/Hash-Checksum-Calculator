#include "PrintVisitor.h"

std::string PrintVisitor::getIndent() const {
    return std::string(indentLevel * 2, ' ');
}

void PrintVisitor::visit(File* file) {
    std::cout << getIndent() << "- [File] " << file->getName()
        << " (" << file->getSize() << " bytes)" << std::endl;
}

void PrintVisitor::visit(Directory* dir) {
    std::cout << getIndent() << "[" << dir->getName() << "]" << std::endl;

    indentLevel++;

    for (const std::unique_ptr<SystemItem>& child : this->getChildren(dir)) {
        child->accept(this);
    }

    indentLevel--;
}

PrinterBuilder::PrinterBuilder()
{
    VisitorBuilder::add(this->clone());
}

std::string PrinterBuilder::getFLag()
{
    return "--print";
}

void PrinterBuilder::action(int argc, char* argv[], std::unique_ptr<ChecksumCalculator>& calc, 
    std::unique_ptr<SystemItem>& root)
{
    std::cout << std::endl << "--- Mode: Structure Print ---" << std::endl;
    PrintVisitor printer;
    root->accept(&printer);
}

std::unique_ptr<VisitorBuilder> PrinterBuilder::clone() const
{
    return std::make_unique<PrinterBuilder>(*this);
}

static PrinterBuilder __;