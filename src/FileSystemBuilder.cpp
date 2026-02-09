#include "FileSystemBuilder.h"
#include "Directory.h"

void FileSystemBuilder::addChildToDir(std::unique_ptr<Directory>& dir, std::unique_ptr<SystemItem> item)
{
	dir->children.push_back(std::move(item));
}
