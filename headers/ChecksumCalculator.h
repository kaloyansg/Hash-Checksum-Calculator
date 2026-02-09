#pragma once
#include "Observable.h"
#include "PauseController.h"
#include <iostream>
class ChecksumCalculator : public Observable
{
public:
	virtual std::string calculate(std::istream&) const = 0;

	void setController(PauseController* c);

protected:
	void checkPause() const;

	PauseController* controller = nullptr;
};

class CalculatorBuilder
{
private:
	static std::vector<std::unique_ptr<CalculatorBuilder>>& getBuilders() {
		static std::vector<std::unique_ptr<CalculatorBuilder>> builders;
		return builders;
	}

protected:
	static void add(std::unique_ptr<CalculatorBuilder> builder);

public:
	virtual ~CalculatorBuilder() = default;
	virtual std::unique_ptr<CalculatorBuilder> clone() const = 0;

	static std::unique_ptr<ChecksumCalculator> of(std::string flag);

protected:
	virtual std::string getFLag() = 0;
	virtual std::unique_ptr<ChecksumCalculator> create() = 0;
};