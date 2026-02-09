#include "XML_RW.h"
#include <fstream>

XML_RW::XML_RW(std::unique_ptr<ChecksumCalculator> calc)
	: ReportWriter(std::move(calc)) {
}

void XML_RW::processFile(File* file)
{
	std::ifstream fileStream(file->getPath(), std::ios::binary);
	if (!fileStream.is_open()) {
		throw std::exception("File could not be opened");
	}
	std::string checksum = calculator->calculate(fileStream);

	std::cout << getIndent() << "<item>" << std::endl;
	ProcessingVisitor::indentLevel++;
	std::cout << getIndent() << "<mode>binary</mode>" << std::endl;
	std::cout << getIndent() << "<checksum>" << checksum << "</checksum>" << std::endl;
	std::cout << getIndent() << "<path>" << file->getPath().string() << "</path>" << std::endl;
	std::cout << getIndent() << "<size>" << file->getSize() << "</size>" << std::endl;
	ProcessingVisitor::indentLevel--;
	std::cout << getIndent() << "</item>" << std::endl;
}

void XML_RW::processDir(Directory* dir)
{
	std::cout << getIndent() << "<checksum>" << std::endl;
	ProcessingVisitor::processDir(dir);
	std::cout << getIndent() << "</checksum>" << std::endl;
}