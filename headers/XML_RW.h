#pragma once
#include "ReportWriter.h"

class XML_RW : public ReportWriter
{
public:
	XML_RW(std::unique_ptr<ChecksumCalculator> calc);
protected:
	void processFile(File* file) override;
	void processDir(Directory* dir) override;
};