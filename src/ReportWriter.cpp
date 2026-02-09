#include "ReportWriter.h"
#include "XML_RW.h"
#include "Default_RW.h"

ReportWriter::ReportWriter(std::unique_ptr<ChecksumCalculator> calc)
    : calculator(std::move(calc)) {
}

ReportBuilder::ReportBuilder()
{
    VisitorBuilder::add(this->clone());
}

std::string ReportBuilder::getFLag()
{
    return "--report";
}

void ReportBuilder::action(int argc, char* argv[], std::unique_ptr<ChecksumCalculator>& calc, 
    std::unique_ptr<SystemItem>& root)
{
    if (calc == nullptr) {
        throw std::exception("Invalid argument flag");
    }
    std::cout << std::endl << "--- Mode: Structure Report ---" << std::endl;

    std::unique_ptr<ReportWriter> rw;
    if (argc >= 4 && std::string(argv[3]) == "-x") {
        rw = std::make_unique<XML_RW>(std::move(calc));
    }
    else {
        rw = std::make_unique<Default_RW>(std::move(calc));
    }
    root->accept(rw.get());
}

std::unique_ptr<VisitorBuilder> ReportBuilder::clone() const
{
    return std::make_unique<ReportBuilder>(*this);
}

static ReportBuilder __;