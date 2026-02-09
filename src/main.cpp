#include <iostream>

#include "SimpleFileSystemBuilder.h"
#include "SmartFileSystemBuilder.h"
#include "ChecksumCalculator.h"
#include "Visitor.h"

const std::string GREEN = "\033[32m";
const std::string YELLOW = "\033[33m";
const std::string RED = "\033[31m";
const std::string BLUE = "\033[36m";
const std::string RESET = "\033[0m";

void printUsage() {
    std::cout << YELLOW << std::endl << "Usage: " << "Checksum"<< " [FLAG]" << std::endl;
    std::cout << "Flags:" << std::endl;
    std::cout << "  --calculate  : Calculate checksums" << std::endl;
    std::cout << "  --report     : Print a report of the directory structure" << std::endl;
    std::cout << "  --print      : Print directory structure only" << std::endl;
    std::cout << "  --checksums  : Compare checksums with a source file" << std::endl;
    std::cout << "  --save       : Save checksums in source file" << RESET << std::endl;
}

void invalidAlgFlag() {
    std::cerr << YELLOW << "(invalid or missing algorithm flag)" << std::endl
        << "Flags:    name                    sign" << std::endl
        << "          Alder32_CC              -a" << std::endl
        << "          CRC32_CC                -c" << std::endl
        << RESET << std::endl;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printUsage();
        return 1;
    }

    std::filesystem::path currentPath = std::filesystem::current_path();
    std::string flag = argv[1];
    if (flag.find('=') != std::string::npos) {
        currentPath = std::filesystem::path(flag.substr(flag.find('=')+1));
    }
    SmartFileSystemBuilder builder;
    std::unique_ptr<SystemItem> root;
    try {
        root = builder.build(currentPath);
    }
    catch (const std::exception& e) {
        std::cerr << currentPath.string() << std::endl;
        std::cerr << RED << e.what() << RESET << std::endl;
        return -1;
    }
    if (!root) {
        std::cerr << RED << "Failed to build file structure." << RESET << std::endl;
        return -1;
    }

    std::unique_ptr<ChecksumCalculator> calc = nullptr;
    if (argc >= 3) {
        std::string algFlag(argv[2]);
        try {
            calc = CalculatorBuilder::of(algFlag);
        } 
        catch (std::exception& e) {
            std::cout << RED << e.what() << RESET << std::endl;
            return -1;
        }
    }

    try {
        VisitorBuilder::apply(argc, argv, calc, root);
    }
    catch (const std::exception& e) {
        std::cerr << RED << e.what() << RESET << std::endl;
        printUsage();
        invalidAlgFlag();
        return 1;
    }
    return 0;
}

//"D:\Documents\FMI\c++\designe patterns\homeworks\homework3\Checksum\Checksum\x64\Debug\Checksum.exe" --calculate="C:\Users\Koko\Desktop\testDir" -a"
//"D:\Documents\FMI\c++\designe patterns\homeworks\homework3\Checksum\Checksum\x64\Debug\Checksum.exe" --checksums="C:\Users\Koko\Desktop\testDir" -a "C:\Users\Koko\Desktop\savedChecksum.txt"