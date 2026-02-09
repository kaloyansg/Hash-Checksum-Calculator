#include "Comperator.h"
#include "File.h"

Comperator::Comperator(std::unique_ptr<ChecksumCalculator> calc, const std::filesystem::path& path)
    : calculator(std::move(calc)) {
    if (!std::filesystem::is_regular_file(path) || path.empty())
    {
        throw std::exception("Invalid source file");
    }

    std::ifstream file(path);
    if (!file.is_open()) {
        throw std::exception("Could not open source file");
    }

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;

        size_t firstQuote = line.find('"');
        size_t lastQuote = line.rfind('"');

        if (firstQuote != std::string::npos && lastQuote != std::string::npos && lastQuote > firstQuote) {

            std::string filePath = line.substr(firstQuote + 1, lastQuote - firstQuote - 1);

            // find first character that is not a space after the last quote
            size_t checksumStart = line.find_first_not_of(" \t", lastQuote + 1);

            if (checksumStart != std::string::npos) {
                std::string checksum = line.substr(checksumStart);

                if (!checksum.empty() && checksum.back() == '\r') {
                    checksum.pop_back();
                }
                content.push_back({ filePath, checksum, false });
            }
        }
    }
}

Comperator::~Comperator()
{
    for (const Comperator::SnapshotEntry& entry : content) {
        if (!entry.visited) {
            std::cout << entry.path << ": " << RED << "REMOVED" << RESET << std::endl;
        }
    }
}

void Comperator::visit(File* file)
{
    bool foundInSnapshot = false;
    std::string currentPath = file->getPath().string();

    for (Comperator::SnapshotEntry& entry : content) {
        if (entry.path == currentPath) {
            entry.visited = true; // mark seen
            foundInSnapshot = true;

            std::ifstream stream(file->getPath(), std::ios::binary);
            if (!stream.is_open()) {
                std::cerr << RED << "[Error] Cannot read: " << currentPath << RESET << std::endl;
                return;
            }

            std::string currentChecksum = calculator->calculate(stream);

            if (entry.checksum == currentChecksum) {
                std::cout << currentPath << ": " << GREEN << "OK" << RESET << std::endl;
            }
            else {
                std::cout << currentPath << ": " << YELLOW << "MODIFIED" << RESET << std::endl;
            }
            break;
        }
    }

    if (!foundInSnapshot) {
        std::cout << currentPath << ": " << YELLOW << "NEW" << RESET << std::endl;
    }
}

void Comperator::visit(Directory* dir)
{
    for (const std::unique_ptr<SystemItem>& child : this->getChildren(dir)) {
        child->accept(this);
    }
}

ComperatorBuilder::ComperatorBuilder()
{
    VisitorBuilder::add(this->clone());
}

std::string ComperatorBuilder::getFLag()
{
    return "--checksums";
}

void ComperatorBuilder::action(int argc, char* argv[], std::unique_ptr<ChecksumCalculator>& calc, 
    std::unique_ptr<SystemItem>& root)
{
    if (calc == nullptr) {
        throw std::exception("Invalid algorithm flag");
    }
    if (argc < 4) {
        throw std::exception("Must point a source file for the comparison");
    }
    Comperator comp(std::move(calc), argv[3]);
    root->accept(&comp);
}

std::unique_ptr<VisitorBuilder> ComperatorBuilder::clone() const
{
    return std::make_unique<ComperatorBuilder>(*this);
}

static ComperatorBuilder __;