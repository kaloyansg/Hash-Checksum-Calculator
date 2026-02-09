#include "File.h"
#include "Visitor.h"
#include <iostream>
#include <fstream>

File::File(const std::string& name, const std::filesystem::path& path) : SystemItem(name, path) {
    std::ifstream temp(path, std::ios::binary);
    if (!temp) {
        throw std::exception("File no existing");
    }
    temp.seekg(0, std::ios::end);
    size = temp.tellg();
    temp.close();
}

void File::accept(Visitor* visitor) {
    visitor->visit(this);
}

size_t File::getSize() const {
    return size;
}

//void File::print(int indentLevel) const {
//    std::string indent(indentLevel * 2, ' ');
//    std::cout << indent << "- " << name << " (" << size << " bytes)" << std::endl;
//}
//
//void File::process() {
//    // Here you would call your ChecksumCalculator
//    // logic that we wrote in the previous step.
//    std::cout << "Calculating checksum for file: " << name << std::endl;
//    // simulate work...
//}
