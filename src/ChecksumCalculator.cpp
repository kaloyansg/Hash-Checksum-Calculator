#include "ChecksumCalculator.h"
#include "Visitor.h"

void CalculatorBuilder::add(std::unique_ptr<CalculatorBuilder> builder) {
	std::vector<std::unique_ptr<CalculatorBuilder>>& builders = getBuilders();
	for (std::unique_ptr<CalculatorBuilder>& curr : builders) {
		if (curr->getFLag() == builder->getFLag()) {
			return;
		}
	}

	builders.push_back(std::move(builder));
}

std::unique_ptr<ChecksumCalculator> CalculatorBuilder::of(std::string flag) {
	std::vector<std::unique_ptr<CalculatorBuilder>>& builders = getBuilders();
	for (std::unique_ptr<CalculatorBuilder>& builder : builders) {
		if (flag == builder->getFLag())
		{
			return builder->create();
		}
	}
	throw std::exception("Invalid algorithm flag");
}

void ChecksumCalculator::setController(PauseController* c) {
	controller = c;
}

void ChecksumCalculator::checkPause() const {
	if (controller)
		controller->checkWait();
}
