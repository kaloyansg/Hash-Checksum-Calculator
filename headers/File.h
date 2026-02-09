#pragma once
#include "SystemItem.h"

class File : public SystemItem {
    size_t size;

public:
    File(const std::string& name, const std::filesystem::path& path);

    void accept(Visitor* visitor) override;

    size_t getSize() const override;
    //void print(int indentLevel) const override;
    //void process() override;
};

