#include "FileManager.h"
#include <fstream>
#include <string> 
#include <windows.h>
#include <filesystem>

std::string FileManager::readFile(const std::string& path) {
    std::ifstream f(path);
    return std::string((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>());
}

std::vector<std::string> FileManager::readLines(const std::string& path) {
    std::ifstream file(path);
    std::string line;
    std::vector<std::string> lines;

    while (std::getline(file, line)) {
        if (!line.empty()) lines.push_back(line);
    }

    return lines;
}

