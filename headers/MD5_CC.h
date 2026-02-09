#pragma once
#include "ChecksumCalculator.h"

class MD5_CC : public ChecksumCalculator
{
public:
    std::string calculate(std::istream&) const override;
};

class MD5_Builder : public CalculatorBuilder
{
public:
    MD5_Builder();
    virtual std::unique_ptr<CalculatorBuilder> clone() const override;

protected:
    virtual std::string getFLag() override;
    virtual std::unique_ptr<ChecksumCalculator> create() override;
};