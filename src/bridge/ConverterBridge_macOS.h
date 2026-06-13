#pragma once

#include <string>

// Pure C++ conversion functions (replacing assembly)
std::string ConvertDecToBin(const std::string& decStr);
std::string ConvertBinToDec(const std::string& binStr);
std::string ConvertDecToHex(const std::string& decStr);
std::string ConvertHexToDec(const std::string& hexStr);
std::string ConvertBinToHex(const std::string& binStr);
std::string ConvertHexToBin(const std::string& hexStr);
