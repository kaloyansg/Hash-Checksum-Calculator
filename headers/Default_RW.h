#pragma once
#include "ReportWriter.h"

class Default_RW : public ReportWriter
{
public:
	Default_RW(std::unique_ptr<ChecksumCalculator> calc);
protected:
	void processFile(File* file) override;
	void processDir(Directory* dir) override;
};

