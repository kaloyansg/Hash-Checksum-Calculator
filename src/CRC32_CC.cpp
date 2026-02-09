#include "CRC32_CC.h"

std::string CRC32_CC::calculate(std::istream& file) const
{
    //std::ifstream file(filepath, std::ios::binary);
    if (!file) throw std::exception("File could not be opened"); //return "";

    file.seekg(0, std::ios::end);
    size_t fileSize = file.tellg();
    file.seekg(0, std::ios::beg);

    uint32_t crc = 0xFFFFFFFF;
    const uint32_t polynomial = 0xEDB88320;

    const size_t bufferSize = 1024;
    std::vector<char> buffer(bufferSize);
    size_t totalRead = 0;

    while (file.read(buffer.data(), bufferSize) || file.gcount() > 0) {
        checkPause();////////////////////////////////////////////////////////////////////////

        size_t count = file.gcount();

        for (size_t i = 0; i < count; ++i) {
            uint8_t byte = static_cast<uint8_t>(buffer[i]);
            crc ^= byte;

            for (int j = 0; j < 8; ++j) {
                if (crc & 1) {
                    crc = (crc >> 1) ^ polynomial;
                }
                else {
                    crc = (crc >> 1);
                }
            }
        }

        totalRead += count;
        notifyProgress(totalRead, fileSize);
    }

    uint32_t finalCRC = ~crc;

    const char hexDigits[] = "0123456789ABCDEF";
    std::string result;
    result.reserve(8);

    for (int i = 7; i >= 0; --i) {
        int nibble = (finalCRC >> (i * 4)) & 0xF;
        result += hexDigits[nibble];
    }

    return result;
}

CRC32_Builder::CRC32_Builder() {
    CalculatorBuilder::add(this->clone());
}

std::string CRC32_Builder::getFLag() {
    return "-c";
}

std::unique_ptr<ChecksumCalculator> CRC32_Builder::create() {
    return std::make_unique<CRC32_CC>();
}

std::unique_ptr<CalculatorBuilder> CRC32_Builder::clone() const
{
    return std::make_unique<CRC32_Builder>(*this);
}

static CRC32_Builder __;