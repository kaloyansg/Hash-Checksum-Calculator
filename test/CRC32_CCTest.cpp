#include <catch2/catch_test_macros.hpp>
#include <sstream>
#include <string>
#include <vector>
#include <iomanip>

#include "CRC32_CC.h"

std::string referenceCRC32(const std::string& input) {
    uint32_t crc = 0xFFFFFFFF;
    const uint32_t polynomial = 0xEDB88320;

    for (char c : input) {
        uint32_t byte = static_cast<unsigned char>(c);
        crc ^= byte;
        for (int j = 0; j < 8; ++j) {
            if (crc & 1)
                crc = (crc >> 1) ^ polynomial;
            else
                crc >>= 1;
        }
    }

    crc = ~crc;

    std::stringstream ss;
    ss << std::hex << std::uppercase << std::setfill('0');
    ss << std::setw(8) << crc;

    return ss.str();
}

TEST_CASE("CRC32 Calculation Logic") {
    CRC32_CC calculator;

    SECTION("Matches Reference for standard text") {
        std::string data = "123456789";
        std::stringstream stream(data);

        std::string actual = calculator.calculate(stream);

        std::string expected = referenceCRC32(data);

        REQUIRE(actual == expected);
        REQUIRE(actual == "CBF43926");
    }

    SECTION("Handles Empty Input") {
        std::string data = "";
        std::stringstream stream(data);

        std::string actual = calculator.calculate(stream);
        std::string expected = referenceCRC32(data);

        REQUIRE(actual == expected);
        REQUIRE(actual == "00000000");
    }

    SECTION("Handles Large Input (Buffer check)") {
        std::string data(5000, 'A');
        std::stringstream stream(data);

        std::string actual = calculator.calculate(stream);
        std::string expected = referenceCRC32(data);

        REQUIRE(actual == expected);
    }
}

TEST_CASE("CRC32 Builder Integration") {
    CRC32_Builder builder;

    SECTION("Correct Flag") {
        auto calc = CalculatorBuilder::of("-c");

        REQUIRE(calc != nullptr);

        std::stringstream ss("test");
        REQUIRE(calc->calculate(ss) == referenceCRC32("test"));
    }
}