#include <catch2/catch_test_macros.hpp>
#include <sstream>
#include "ChecksumCalculator.h" 

class MockCalculator : public ChecksumCalculator {
public:
    std::string calculate(std::istream&) const override {
        return "MOCK_HASH";
    }
};

class MockBuilder : public CalculatorBuilder {
public:
    MockBuilder() {
        CalculatorBuilder::add(this->clone());
    }

    std::unique_ptr<CalculatorBuilder> clone() const override {
        return std::make_unique<MockBuilder>(*this);
    }

protected:
    std::string getFLag() override {
        return "-test";
    }

    std::unique_ptr<ChecksumCalculator> create() override {
        return std::make_unique<MockCalculator>();
    }
};

TEST_CASE("CalculatorBuilder: Factory Logic") {
    static MockBuilder registerMock;

    SECTION("Successfully creates a calculator for a known flag") {
        std::unique_ptr<ChecksumCalculator> calc;

        REQUIRE_NOTHROW(calc = CalculatorBuilder::of("-test"));

        REQUIRE(calc != nullptr);

        std::stringstream dummyStream;
        REQUIRE(calc->calculate(dummyStream) == "MOCK_HASH");
    }

    SECTION("Throws exception for unknown flags") {
        REQUIRE_THROWS_AS(CalculatorBuilder::of("-invalid"), std::exception);

        try {
            CalculatorBuilder::of("-invalid");
        }
        catch (const std::exception& e) {
            REQUIRE(std::string(e.what()) == "Invalid algorithm flag");
        }
    }

    SECTION("Is Case Sensitive (sanity check)") {
        REQUIRE_THROWS(CalculatorBuilder::of("-TEST"));
    }
}