#include "Directory.h"
#include "Visitor.h"
#include <iostream>

Directory::Directory(const std::string& name, const std::filesystem::path& path)
    : SystemItem(name, path) {}

void Directory::accept(Visitor* visitor) {
    visitor->visit(this);
}

size_t Directory::getSize() const {
    size_t total = 0;
    for (const std::unique_ptr<SystemItem>& child : children) {
        total += child->getSize();
    }
    return total;
}

//void Directory::print(int indentLevel) const {
//    std::string indent(indentLevel * 2, ' ');
//    std::cout << indent << "[" << name << "]" << std::endl;
//
//    for (const std::unique_ptr<SystemItem>& child : children) {
//        child->print(indentLevel + 1);
//    }
//}
//
//void Directory::process() {
//    std::cout << "Entering directory: " << name << std::endl;
//    for (const std::unique_ptr<SystemItem>& child : children) {
//        child->process();
//    }
//}

//void Directory::add(std::unique_ptr<SystemItem> item) {
//    children.push_back(std::move(item));
//}
