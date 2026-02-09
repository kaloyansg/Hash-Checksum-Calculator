#include <catch2/catch_test_macros.hpp>
#include <sstream>
#include <string>
#include <vector>
#include <iomanip>

#include "MD5_CC.h"
#include <openssl/evp.h>

// open ssl md5
std::string calculateRawOpenSSL(const std::string& input) {
    EVP_MD_CTX* ctx = EVP_MD_CTX_new();
    const EVP_MD* md = EVP_md5();

    unsigned char hash[EVP_MAX_MD_SIZE];
    unsigned int lengthOfHash = 0;

    EVP_DigestInit_ex(ctx, md, NULL);
    EVP_DigestUpdate(ctx, input.c_str(), input.size());
    EVP_DigestFinal_ex(ctx, hash, &lengthOfHash);
    EVP_MD_CTX_free(ctx);

    std::stringstream ss;
    ss << std::hex << std::uppercase << std::setfill('0');
    for (unsigned int i = 0; i < lengthOfHash; ++i) {
        ss << std::setw(2) << (int)hash[i];
    }
    return ss.str();
}

TEST_CASE("MD5 Calculation Logic") {
    MD5_CC calculator;

    SECTION("Matches OpenSSL for standard text") {
        std::string data = "The quick brown fox jumps over the lazy dog";
        std::stringstream stream(data);

        std::string actual = calculator.calculate(stream);

        std::string expected = calculateRawOpenSSL(data);

        REQUIRE(actual == expected);

        REQUIRE(actual == "9E107D9D372BB6826BD81D3542A419D6");
    }

    SECTION("Handles Empty Input") {
        std::string data = "";
        std::stringstream stream(data);

        std::string actual = calculator.calculate(stream);
        std::string expected = calculateRawOpenSSL(data);

        REQUIRE(actual == expected);
        REQUIRE(actual == "D41D8CD98F00B204E9800998ECF8427E");
    }

    SECTION("Handles Large Input (Buffer check)") {
        std::string data(5000, 'A');
        std::stringstream stream(data);

        std::string actual = calculator.calculate(stream);
        std::string expected = calculateRawOpenSSL(data);

        REQUIRE(actual == expected);
    }
}

TEST_CASE("MD5 Builder Integration") {
    MD5_Builder builder;

    SECTION("Correct Flag") {

        auto calc = CalculatorBuilder::of("-m");
        REQUIRE(calc != nullptr);

        std::stringstream ss("test");
        REQUIRE(calc->calculate(ss) == calculateRawOpenSSL("test"));
    }
}