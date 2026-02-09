#pragma once
#include <vector>
#include <iostream>
#include "SystemItem.h"
#include "ChecksumCalculator.h"

class File;
class Directory;

class Visitor {
public:
	Visitor(std::ostream& outStream = std::cout, std::istream& inStream = std::cin)
		: os(outStream), is(inStream) {}
    virtual ~Visitor() = default;

    virtual void visit(File* file) = 0;
    virtual void visit(Directory* directory) = 0;

protected:
    const std::vector<std::unique_ptr<SystemItem>>& getChildren(Directory* dir);

	std::istream& is;
	std::ostream& os;
};

class VisitorBuilder {
private:
	static std::vector<std::unique_ptr<VisitorBuilder>>& getBuilders();

protected:
	static void add(std::unique_ptr<VisitorBuilder> builder);
	virtual std::string getFLag() = 0;
	virtual void action(int argc, char* argv[], std::unique_ptr<ChecksumCalculator>& calc,
		std::unique_ptr<SystemItem>& root) = 0;

public:
	virtual ~VisitorBuilder() = default;
	virtual std::unique_ptr<VisitorBuilder> clone() const = 0;
	static void apply(int argc, char* argv[], std::unique_ptr<ChecksumCalculator>& calc, 
		std::unique_ptr<SystemItem>& root);
};