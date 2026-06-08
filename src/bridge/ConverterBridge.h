#pragma once

#include <cstring>
#include <string>

extern "C" {
void DecToBinAsm(const char *input, char *output);
void BinToDecAsm(const char *input, char *output);

void DecToHexAsm(const char *input, char *output);
void HexToDecAsm(const char *input, char *output);

void BinToHexAsm(const char *input, char *output);
void HexToBinAsm(const char *input, char *output);
}

std::string ConvertDecToBin(const std::string &decStr);
std::string ConvertBinToDec(const std::string &binStr);
std::string ConvertDecToHex(const std::string &decStr);
std::string ConvertHexToDec(const std::string &hexStr);
std::string ConvertBinToHex(const std::string &binStr);
std::string ConvertHexToBin(const std::string &hexStr);