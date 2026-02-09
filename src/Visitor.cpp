#include "Visitor.h"
#include "Directory.h"

// visitor is a friend of directory
const std::vector<std::unique_ptr<SystemItem>>& Visitor::getChildren(Directory* dir) {
    return dir->children;
}

void VisitorBuilder::add(std::unique_ptr<VisitorBuilder> builder) {
    std::vector<std::unique_ptr<VisitorBuilder>>& builders = getBuilders();
    for (std::unique_ptr<VisitorBuilder>& curr : builders) {
        if (curr->getFLag() == builder->getFLag())
        {
            return;
        }
    }
    builders.push_back(std::move(builder));
}

std::vector<std::unique_ptr<VisitorBuilder>>& VisitorBuilder::getBuilders() {
    static std::vector<std::unique_ptr<VisitorBuilder>> visitBuilder;
    return visitBuilder;
}

void VisitorBuilder::apply(int argc, char* argv[], std::unique_ptr<ChecksumCalculator>& calc,
    std::unique_ptr<SystemItem>& root)
{
    std::string flag = argv[1];
    std::vector<std::unique_ptr<VisitorBuilder>>& builders = getBuilders();
    for (std::unique_ptr<VisitorBuilder>& builder : builders) {
        std::string currFlag = builder->getFLag();
        if (flag.substr(0, currFlag.length()) == currFlag) {
            builder->action(argc, argv, calc, root);
            return;
        }
    }
    throw std::exception("invalid argument flag");
}