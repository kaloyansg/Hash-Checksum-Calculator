#pragma once
#include "ChecksumCalculator.h"

class Alder32_CC : public ChecksumCalculator
{
public:
	std::string calculate(std::istream&) const override;
};

class Alder32_Builder : public CalculatorBuilder
{
public:
	Alder32_Builder();
	virtual std::unique_ptr<CalculatorBuilder> clone() const override;

protected:
	virtual std::string getFLag() override;
	virtual std::unique_ptr<ChecksumCalculator> create() override;
};
