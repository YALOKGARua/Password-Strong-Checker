#ifndef UTILS_HPP
#define UTILS_HPP
#include <string>
#include <vector>
#include <random>

namespace Utils {
    std::string toLower(const std::string& str);
    std::string toUpper(const std::string& str);
    std::string trim(const std::string& str);
    std::vector<std::string> split(const std::string& str, char delimiter);
    bool startsWith(const std::string& str, const std::string& prefix);
    bool endsWith(const std::string& str, const std::string& suffix);
    
    std::string generateRandomPassword(size_t length, bool includeUpper = true,
                                     bool includeLower = true, bool includeDigits = true,
                                     bool includeSpecial = true);
    double calculatePasswordEntropy(const std::string& password);
    bool isCommonPassword(const std::string& password,
                         const std::vector<std::string>& commonWords);
    
    bool containsUpperCase(const std::string& str);
    bool containsLowerCase(const std::string& str);
    bool containsDigit(const std::string& str);
    bool containsSpecialChar(const std::string& str);
    bool isSequential(const std::string& str);
    
    int getRandomInt(int min, int max);
    char getRandomChar(const std::string& charset);
}

#endif