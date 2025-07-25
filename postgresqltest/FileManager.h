#pragma once
#include <string>
#include <vector>

class FileManager {
public:

    static std::string readFile(const std::string& path);
    static std::vector<std::string> readLines(const std::string& path);
};
