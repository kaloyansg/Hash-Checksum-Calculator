#include "ProcessingVisitor.h"

std::string ProcessingVisitor::getIndent() const {
    return std::string(indentLevel * 2, ' ');
}

void ProcessingVisitor::processDir(Directory* dir) {
    indentLevel++;
    for (const std::unique_ptr<SystemItem>& child : this->getChildren(dir)) {
        child->accept(this);
    }
    indentLevel--;
}

void ProcessingVisitor::visit(Directory* dir) {
    //std::cout << getIndent() << "[" << dir->getName() << "]" << std::endl;
    processDir(dir);

    /*indentLevel++;
    for (const std::unique_ptr<SystemItem>& child : this->getChildren(dir)) {
        child->accept(this);
    }
    indentLevel--;*/
}

void ProcessingVisitor::visit(File* file) {
    processFile(file);
}
