#include <iostream>
#include <string>

extern "C" {
    void DecToBinAsm(const char* input, char* output);
    void BinToDecAsm(const char* input, char* output);
    void DecToHexAsm(const char* input, char* output);
    void HexToDecAsm(const char* input, char* output);
    void BinToHexAsm(const char* input, char* output);
    void HexToBinAsm(const char* input, char* output);
}

void test(const std::string& name, void (*func)(const char*, char*), const char* input) {
    char output[1024] = {0};
    func(input, output);
    std::cout << name << "(\"" << input << "\") = " << output << std::endl;
}

int main() {
    test("DecToBin", DecToBinAsm, "25");
    test("DecToBin", DecToBinAsm, "-5");
    test("DecToBin", DecToBinAsm, "abc");
    
    test("BinToDec", BinToDecAsm, "11001");
    test("BinToDec", BinToDecAsm, "11111111111111111111111111111011");
    
    test("DecToHex", DecToHexAsm, "255");
    test("DecToHex", DecToHexAsm, "-1");
    
    test("HexToDec", HexToDecAsm, "FF");
    test("HexToDec", HexToDecAsm, "ffffffff");
    test("HexToDec", HexToDecAsm, "FFH");
    test("HexToDec", HexToDecAsm, "ffh");
    test("HexToDec", HexToDecAsm, "H");
    test("HexToBin", HexToBinAsm, "FFH");
    
    return 0;
}
