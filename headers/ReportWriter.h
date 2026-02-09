#pragma once
#include "ProcessingVisitor.h"
#include "ChecksumCalculator.h"

class ReportWriter : public ProcessingVisitor {
public:
    ReportWriter(std::unique_ptr<ChecksumCalculator> calc);
protected:
    std::unique_ptr<ChecksumCalculator> calculator;
};

class ReportBuilder : public VisitorBuilder {
public:
    ReportBuilder();

    virtual std::string getFLag() override;
    virtual void action(int argc, char* argv[], std::unique_ptr<ChecksumCalculator>& calc,
        std::unique_ptr<SystemItem>& root) override;
    virtual std::unique_ptr<VisitorBuilder> clone() const override;
};