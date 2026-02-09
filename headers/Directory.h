#pragma once
#include "SystemItem.h"
//#include "FileSystemBuilder.h"

//class FileSystemBuilder;

class Directory : public SystemItem {
    std::vector<std::unique_ptr<SystemItem>> children;

public:
    Directory(const std::string& name, const std::filesystem::path& path);

    friend class FileSystemBuilder;

    void accept(Visitor* visitor) override;
    friend class Visitor;

    size_t getSize() const override;
    /*void print(int indentLevel) const override;
    void process() override;*/

//private:
//    friend class FileSystemBuilder;
    //void add(std::unique_ptr<SystemItem> item);
};

