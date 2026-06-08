#include "FileReader.h"

#include <fstream>

std::string ReadFileContent(
    const std::string& path)
{
    std::ifstream file(path);

    std::string content;
    std::string line;

    while(getline(file,line))
    {
        content += line;
        content += "\n";
    }

    return content;
}