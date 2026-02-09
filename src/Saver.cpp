#include "Saver.h"
#include "File.h"

Saver::Saver(std::unique_ptr<ChecksumCalculator> calc, const std::filesystem::path& path)
	: calculator(std::move(calc)){
	writer.open(path);
	if (!writer.is_open()) {
		throw std::exception("Could not open source file");
	}
}

Saver::~Saver()
{
	if (writer.is_open())
		writer.close();
}

void Saver::visit(File* file)
{
	std::ifstream stream(file->getPath(), std::ios::binary);
	if (!stream.is_open()) {
		std::cerr << RED << "[Error] Could not open: " << file->getPath().string() << RESET << std::endl;
		return;
	}
	std::string checksum = calculator->calculate(stream);

	writer << "\"" << file->getPath().string() << "\" " << checksum << std::endl;
	writer.flush();
}

void Saver::visit(Directory* dir)
{
	for (const std::unique_ptr<SystemItem>& child : this->getChildren(dir)) {
		child->accept(this);
	}
}

SaverBuilder::SaverBuilder()
{
	VisitorBuilder::add(this->clone());
}

std::string SaverBuilder::getFLag()
{
	return "--save";
}

void SaverBuilder::action(int argc, char* argv[], std::unique_ptr<ChecksumCalculator>& calc, 
	std::unique_ptr<SystemItem>& root)
{
	if (calc == nullptr) {
		throw std::exception("Invalid algorithm flag!");
	}
	if (argc < 4) {
		throw std::exception("Must point a source file where to save the checksums!");
	}
	std::cout << std::endl << "--- Mode: Saving Checksums ---" << std::endl;
	Saver saver(std::move(calc), argv[3]);
	root->accept(&saver);
	std::cout << GREEN << "-- SAVED --" << RESET << std::endl;
}

std::unique_ptr<VisitorBuilder> SaverBuilder::clone() const
{
	return std::make_unique<SaverBuilder>(*this);
}

static SaverBuilder __;