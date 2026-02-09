#include "MD5_CC.h"
#include <vector>
#include <iomanip>
#include <sstream>

#include <openssl/evp.h>

std::string MD5_CC::calculate(std::istream& file) const
{
    if (!file) throw std::exception("File could not be opened"); //return "";

    file.seekg(0, std::ios::end);
    size_t fileSize = file.tellg();
    file.seekg(0, std::ios::beg);

    // md5
    EVP_MD_CTX* ctx = EVP_MD_CTX_new();
    if (!ctx) throw std::exception("Could"); //return "";

    if (1 != EVP_DigestInit_ex(ctx, EVP_md5(), NULL)) {
        EVP_MD_CTX_free(ctx);
        return "";
    }

    const size_t bufferSize = 1024;
    std::vector<char> buffer(bufferSize);
    size_t totalRead = 0;

    while (file.read(buffer.data(), bufferSize) || file.gcount() > 0) {
        checkPause();

        size_t count = file.gcount();

        if (1 != EVP_DigestUpdate(ctx, buffer.data(), count)) {
            EVP_MD_CTX_free(ctx);
            return "";
        }

        totalRead += count;
        notifyProgress(totalRead, fileSize);
    }

    unsigned char hash[EVP_MAX_MD_SIZE];
    unsigned int lengthOfHash = 0;

    if (1 != EVP_DigestFinal_ex(ctx, hash, &lengthOfHash)) {
        EVP_MD_CTX_free(ctx);
        return "";
    }

    EVP_MD_CTX_free(ctx);

    std::stringstream ss;
    ss << std::hex << std::uppercase << std::setfill('0');
    for (unsigned int i = 0; i < lengthOfHash; ++i) {
        ss << std::setw(2) << (int)hash[i];
    }

    return ss.str();
}

MD5_Builder::MD5_Builder()
{
    CalculatorBuilder::add(this->clone());
}

std::unique_ptr<CalculatorBuilder> MD5_Builder::clone() const
{
    return std::make_unique<MD5_Builder>(*this);
}

std::string MD5_Builder::getFLag()
{
    return "-m";
}

std::unique_ptr<ChecksumCalculator> MD5_Builder::create()
{
    return std::make_unique<MD5_CC>();
}

static MD5_Builder __;