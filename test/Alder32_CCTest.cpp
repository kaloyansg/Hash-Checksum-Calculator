#include <catch2/catch_test_macros.hpp>
#include <sstream>
#include <string>
#include <vector>
#include <iomanip>

#include "Alder32_CC.h"

std::string referenceAdler32(const std::string& input) {
    uint32_t a = 1;
    uint32_t b = 0;
    const uint32_t MOD_ADLER = 65521;

    for (char c : input) {
        a = (a + (unsigned char)c) % MOD_ADLER;
        b = (b + a) % MOD_ADLER;
    }

    uint32_t checksum = (b << 16) | a;

    std::stringstream ss;
    ss << std::hex << std::uppercase << std::setfill('0');
    ss << std::setw(8) << checksum;

    return ss.str();
}


TEST_CASE("Alder32 Calculation Logic") {
    Alder32_CC calculator;

    SECTION("Matches Reference for standard text") {
        std::string data = "Wikipedia";
        std::stringstream stream(data);

        std::string actual = calculator.calculate(stream);

        std::string expected = referenceAdler32(data);

        REQUIRE(actual == expected);
        REQUIRE(actual == "11E60398");
    }

    SECTION("Handles Empty Input") {
        std::string data = "";
        std::stringstream stream(data);

        std::string actual = calculator.calculate(stream);
        std::string expected = referenceAdler32(data);

        REQUIRE(actual == expected);
        REQUIRE(actual == "00000001");
    }

    SECTION("Handles Large Input (Buffer check)") {
        std::string data(5000, 'Z');
        std::stringstream stream(data);

        std::string actual = calculator.calculate(stream);
        std::string expected = referenceAdler32(data);

        REQUIRE(actual == expected);
    }
}

TEST_CASE("Alder32 Builder Integration") {
    Alder32_Builder builder;

    SECTION("Correct Flag") {
        auto calc = CalculatorBuilder::of("-a");

        REQUIRE(calc != nullptr);
        std::stringstream ss("Wiki");
        REQUIRE(calc->calculate(ss) == referenceAdler32("Wiki"));
    }
}