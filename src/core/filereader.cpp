#include "core/filereader.hpp"
#include <spdlog/spdlog.h>

using namespace sg::Core;

// Helper methods
std::vector<std::string> FileReader::split(std::string_view str, char delim)
{
    std::vector<std::string> result;
    auto left = str.begin();
    for (auto& it = left; it != str.end(); ++it)
    {
        if (*it == delim)
        {
            result.emplace_back(&*left, it - left);
            left = it + 1;
        }
    }
    if (left != str.end())
        result.emplace_back(&*left, str.end() - left);
    return result;
}

int FileReader::safe_stoi(const std::string& s)
{
    try {
        return std::stoi(s);
    }
    catch (const std::exception&) {
        spdlog::error("safe_stoi failed on token: '{}'", s);
        return 0;
    }
}

std::vector<int> FileReader::split_ints(const std::string& s, const std::string& delimiter)
{
    std::vector<int> tokens;
    size_t start = 0;
    size_t pos = 0;

    while ((pos = s.find(delimiter, start)) != std::string::npos)
    {
        std::string part = s.substr(start, pos - start);
        tokens.push_back(part.empty() ? 0 : safe_stoi(part));
        start = pos + delimiter.length();
    }

    std::string part = s.substr(start);
    tokens.push_back(part.empty() ? 0 : safe_stoi(part));

    return tokens;
}

std::string FileReader::ReadFile(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        spdlog::error("Failed to open file: '{}'", filePath);
        return "";
    }

    std::string content((std::istreambuf_iterator<char>(file)),
        std::istreambuf_iterator<char>());
    return content;
}

bool FileReader::WriteFile(const std::string& filepath, const std::string& content)
{
    std::ofstream file(filepath);

    // Open file
    if (!file.is_open()) {
        spdlog::error("Failed to write file to path: '%s'", filepath);
        return false;
    }

    // Write to opened file
    file << content;

    file.close();

    return true;
}