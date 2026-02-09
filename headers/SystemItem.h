#pragma once
#include <string>
#include <filesystem>
#include <iostream>

class Visitor;

class SystemItem {
protected:
    std::string name;
    std::filesystem::path path;

public:
    SystemItem(const std::string& name, const std::filesystem::path& path);
    virtual ~SystemItem() = default;

    virtual void accept(Visitor* visitor) = 0;

    std::string getName() const;
    std::filesystem::path getPath() const;

    virtual size_t getSize() const = 0;
};