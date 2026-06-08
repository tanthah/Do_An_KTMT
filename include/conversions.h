#pragma once
#include <string>
#include <cstdint>

// Conversion wrappers (C++ side) that call assembly implementations.

std::string decToBin(uint32_t decimal);
uint32_t binToDec(const std::string& binary);
std::string decToHex(uint32_t decimal);
uint32_t hexToDec(const std::string& hexStr);
std::string binToHex(const std::string& binary);
std::string hexToBin(const std::string& hexStr);
