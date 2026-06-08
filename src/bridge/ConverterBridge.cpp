#include "ConverterBridge.h"
#include <vector>
#include <algorithm>
#include <cctype>

static std::string TrimString(const std::string& s)
{
    auto start = s.begin();
    while (start != s.end() && std::isspace((unsigned char)*start)) {
        start++;
    }
    auto end = s.end();
    if (start == s.end()) return "";
    do {
        end--;
    } while (std::distance(start, end) > 0 && std::isspace((unsigned char)*end));
    return std::string(start, end + 1);
}

std::string ConvertDecToBin(const std::string& decStr)
{
    std::string s = TrimString(decStr);
    if (s.empty()) return "Du lieu khong hop le";
    char output[1024] = {0};
    DecToBinAsm(s.c_str(), output);
    return std::string(output);
}

std::string ConvertBinToDec(const std::string& binStr)
{
    std::string s = TrimString(binStr);
    if (s.empty()) return "Du lieu khong hop le";
    char output[1024] = {0};
    BinToDecAsm(s.c_str(), output);
    return std::string(output);
}

std::string ConvertDecToHex(const std::string& decStr)
{
    std::string s = TrimString(decStr);
    if (s.empty()) return "Du lieu khong hop le";
    char output[1024] = {0};
    DecToHexAsm(s.c_str(), output);
    return std::string(output);
}

std::string ConvertHexToDec(const std::string& hexStr)
{
    std::string s = TrimString(hexStr);
    if (s.empty()) return "Du lieu khong hop le";
    char output[1024] = {0};
    HexToDecAsm(s.c_str(), output);
    return std::string(output);
}

std::string ConvertBinToHex(const std::string& binStr)
{
    std::string s = TrimString(binStr);
    if (s.empty()) return "Du lieu khong hop le";
    char output[1024] = {0};
    BinToHexAsm(s.c_str(), output);
    return std::string(output);
}

std::string ConvertHexToBin(const std::string& hexStr)
{
    std::string s = TrimString(hexStr);
    if (s.empty()) return "Du lieu khong hop le";
    char output[1024] = {0};
    HexToBinAsm(s.c_str(), output);
    return std::string(output);
}