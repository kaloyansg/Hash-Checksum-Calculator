#include "Alder32_CC.h"


std::string Alder32_CC::calculate(std::istream& file) const
{
    //std::ifstream file(filepath, std::ios::binary);
    if (!file) throw std::exception("File could not be opened"); //return "";

    file.seekg(0, std::ios::end);
    size_t fileSize = file.tellg();
    file.seekg(0, std::ios::beg);

    uint32_t a = 1, b = 0;
    const size_t bufferSize = 1024;
    std::vector<char> buffer(bufferSize);
    size_t totalRead = 0;

    while (file.read(buffer.data(), bufferSize) || file.gcount() > 0) {
        checkPause();////////////////////////////////////////////////////////////////////////

        size_t count = file.gcount();
        for (size_t i = 0; i < count; ++i) {
            a = (a + (unsigned char)buffer[i]) % 65521;
            b = (b + a) % 65521;
        }

        totalRead += count;
        notifyProgress(totalRead, fileSize);
    }

    uint32_t checksum = (b << 16) | a;

    const char hexDigits[] = "0123456789ABCDEF";
    std::string result;
    result.reserve(8);

    for (int i = 7; i >= 0; --i) {
        int nibble = (checksum >> (i * 4)) & 0xF;
        result += hexDigits[nibble];
    }

    return result;
}

Alder32_Builder::Alder32_Builder() {
    CalculatorBuilder::add(this->clone());
}

std::string Alder32_Builder::getFLag() {
    return "-a";
}

std::unique_ptr<ChecksumCalculator> Alder32_Builder::create() {
    return std::make_unique<Alder32_CC>();
}

std::unique_ptr<CalculatorBuilder> Alder32_Builder::clone() const
{
    return std::make_unique<Alder32_Builder>(*this);
}

static Alder32_Builder __;