#pragma once
#include "ChecksumCalculator.h"
class CRC32_CC : public ChecksumCalculator
{
public:
	std::string calculate(std::istream&) const override;
};

class CRC32_Builder : public CalculatorBuilder
{
public:
	CRC32_Builder();
	virtual std::unique_ptr<CalculatorBuilder> clone() const override;

protected:
	virtual std::string getFLag() override;
	virtual std::unique_ptr<ChecksumCalculator> create() override;
};
