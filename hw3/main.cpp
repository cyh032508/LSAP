#include "Huffman.h"

#define PROGRAM_NAME "b11705044-compression" 

void printUsage() {
    std::cout << "Usage:" << std::endl;
    std::cout << "  " << PROGRAM_NAME << " -c <input_file> <output_file>" << std::endl;
    std::cout << "    -c: Compress <input_file> using Huffman coding." << std::endl;
    std::cout << "    Example: " << PROGRAM_NAME << " -c test.txt test.huf" << std::endl;
    std::cout << std::endl;
    std::cout << "  " << PROGRAM_NAME << " -d <input_file> <output_file>" << std::endl;
    std::cout << "    -d: Decompress <input_file> (a Huffman file)." << std::endl;
    std::cout << "    Example: " << PROGRAM_NAME << " -d test.huf test_decoded.txt" << std::endl;
    std::cout << std::endl;
    std::cout << "  " << PROGRAM_NAME << " -h | --help" << std::endl;
    std::cout << "    Display this help message." << std::endl;
}

int main(int argc, char* argv[]) {
    // 檢查引數數量
    if (argc < 2 || argc > 4) {
        printUsage();
        return 1;
    }

    std::string mode = argv[1];
    
    // 處理幫助選項
    if (mode == "-h" || mode == "--help") {
        printUsage();
        return 0;
    }

    // 檢查壓縮/解壓縮模式所需的引數數量
    if (argc != 4) {
        std::cerr << "Error: Invalid number of arguments for mode " << mode << "." << std::endl;
        printUsage();
        return 1;
    }

    std::string inputFile = argv[2];
    std::string outputFile = argv[3];
    
    Huffman huffman;
    bool success = false;

    if (mode == "-c") {
        std::cout << "Starting compression: " << inputFile << " -> " << outputFile << std::endl;
        success = huffman.compress(inputFile, outputFile);
    } 
    else if (mode == "-d") {
        std::cout << "Starting decompression: " << inputFile << " -> " << outputFile << std::endl;
        success = huffman.decompress(inputFile, outputFile);
    } 
    else {
        std::cerr << "Error: Invalid mode specified: " << mode << std::endl;
        printUsage();
        return 1;
    }

    return success ? 0 : 1;
}