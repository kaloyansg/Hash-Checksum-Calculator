#include "Default_RW.h"
#include <fstream>

Default_RW::Default_RW(std::unique_ptr<ChecksumCalculator> calc)
    : ReportWriter(std::move(calc)) {
}

void Default_RW::processFile(File* file) {
    std::ifstream fileStream(file->getPath(), std::ios::binary);

    if (!fileStream.is_open()) {
        throw std::exception("File could not be opened");
    }
    
    std::string checksum = calculator->calculate(fileStream);

    std::cout << checksum << " *" << file->getPath().string() << std::endl;

    //std::cout << getIndent() << "- " << file->getName()
      //  << " (Size: " << file->getSize() << " bytes)" << ", (Checksum: " << checksum << ")" << std::endl;
}

void Default_RW::processDir(Directory* dir)
{
    //std::cout << getIndent() << "[" << dir->getName() << "]" << std::endl;
    ProcessingVisitor::processDir(dir);
}
