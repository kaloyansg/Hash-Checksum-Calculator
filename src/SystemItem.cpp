#include "SystemItem.h"

SystemItem::SystemItem(const std::string& name, const std::filesystem::path& path) : name(name), path(path) {}

std::string SystemItem::getName() const { 
	return name;
}

std::filesystem::path SystemItem::getPath() const { 
	return path; 
}
