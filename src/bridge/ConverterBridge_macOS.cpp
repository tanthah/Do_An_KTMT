#include "ConverterBridge_macOS.h"
#include <sstream>
#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <cstdint>

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
    
    // Handle negative numbers
    bool negative = false;
    if (s[0] == '-') {
        negative = true;
        s = s.substr(1);
    }
    
    try {
        long long dec = std::stoll(s);
        if (dec == 0) return "0";
        
        std::string bin;
        unsigned long long num = (dec < 0) ? -dec : dec;
        while (num > 0) {
            bin = (char)('0' + (num % 2)) + bin;
            num /= 2;
        }
        if (dec < 0) bin = "-" + bin;
        return bin;
    } catch (...) {
        return "Du lieu khong hop le";
    }
}

std::string ConvertBinToDec(const std::string& binStr)
{
    std::string s = TrimString(binStr);
    if (s.empty()) return "Du lieu khong hop le";
    
    bool negative = false;
    if (s[0] == '-') {
        negative = true;
        s = s.substr(1);
    }
    
    try {
        long long dec = 0;
        for (char c : s) {
            if (c != '0' && c != '1') return "Du lieu khong hop le";
            dec = dec * 2 + (c - '0');
        }
        if (negative) dec = -dec;
        return std::to_string(dec);
    } catch (...) {
        return "Du lieu khong hop le";
    }
}

std::string ConvertDecToHex(const std::string& decStr)
{
    std::string s = TrimString(decStr);
    if (s.empty()) return "Du lieu khong hop le";
    
    bool negative = false;
    if (s[0] == '-') {
        negative = true;
        s = s.substr(1);
    }
    
    try {
        long long dec = std::stoll(s);
        std::ostringstream oss;
        oss << std::hex << (dec < 0 ? -dec : dec);
        std::string hex = oss.str();
        if (negative) hex = "-" + hex;
        return hex;
    } catch (...) {
        return "Du lieu khong hop le";
    }
}

std::string ConvertHexToDec(const std::string& hexStr)
{
    std::string s = TrimString(hexStr);
    if (s.empty()) return "Du lieu khong hop le";
    
    bool negative = false;
    if (s[0] == '-') {
        negative = true;
        s = s.substr(1);
    }
    
    try {
        long long dec = std::stoll(s, nullptr, 16);
        if (negative) dec = -dec;
        return std::to_string(dec);
    } catch (...) {
        return "Du lieu khong hop le";
    }
}

std::string ConvertBinToHex(const std::string& binStr)
{
    // Convert via decimal
    std::string dec = ConvertBinToDec(binStr);
    if (dec.find("Du lieu khong hop le") != std::string::npos) return dec;
    return ConvertDecToHex(dec);
}

std::string ConvertHexToBin(const std::string& hexStr)
{
    // Convert via decimal
    std::string dec = ConvertHexToDec(hexStr);
    if (dec.find("Du lieu khong hop le") != std::string::npos) return dec;
    return ConvertDecToBin(dec);
}
