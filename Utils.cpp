#include "Utils.hpp"
#include <algorithm>
#include <random>
#include <chrono>
#include <sstream>
#include <cctype>
#include <cmath>
#include <set>

namespace Utils {
    std::string toLower(const std::string& str) {
        std::string result = str;
        std::transform(result.begin(), result.end(), result.begin(),
                      [](unsigned char c) { return std::tolower(c); });
        return result;
    }

    std::string toUpper(const std::string& str) {
        std::string result = str;
        std::transform(result.begin(), result.end(), result.begin(),
                      [](unsigned char c) { return std::toupper(c); });
        return result;
    }

    std::string trim(const std::string& str) {
        const std::string whitespace = " \t\n\r\f\v";
        size_t start = str.find_first_not_of(whitespace);
        if (start == std::string::npos) return "";
        size_t end = str.find_last_not_of(whitespace);
        return str.substr(start, end - start + 1);
    }

    std::vector<std::string> split(const std::string& str, char delimiter) {
        std::vector<std::string> tokens;
        std::stringstream ss(str);
        std::string token;
        while (std::getline(ss, token, delimiter)) {
            if (!token.empty()) tokens.push_back(token);
        }
        return tokens;
    }

    bool startsWith(const std::string& str, const std::string& prefix) {
        return str.size() >= prefix.size() && str.compare(0, prefix.size(), prefix) == 0;
    }

    bool endsWith(const std::string& str, const std::string& suffix) {
        return str.size() >= suffix.size() && str.compare(str.size() - suffix.size(), suffix.size(), suffix) == 0;
    }

    std::string generateRandomPassword(size_t length, bool includeUpper,
                                     bool includeLower, bool includeDigits,
                                     bool includeSpecial) {
        static const std::string upper = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
        static const std::string lower = "abcdefghijklmnopqrstuvwxyz";
        static const std::string digits = "0123456789";
        static const std::string special = "!@#$%^&*()_+-=[]{}|;:,.<>?";

        std::string charset;
        if (includeUpper) charset += upper;
        if (includeLower) charset += lower;
        if (includeDigits) charset += digits;
        if (includeSpecial) charset += special;

        if (charset.empty()) throw std::invalid_argument("At least one character set must be included");

        std::string password;
        password.reserve(length);

        if (includeUpper) password += getRandomChar(upper);
        if (includeLower) password += getRandomChar(lower);
        if (includeDigits) password += getRandomChar(digits);
        if (includeSpecial) password += getRandomChar(special);

        while (password.length() < length) password += getRandomChar(charset);

        std::random_device rd;
        std::mt19937 gen(rd());
        std::shuffle(password.begin(), password.end(), gen);

        return password;
    }

    double calculatePasswordEntropy(const std::string& password) {
        std::set<char> charset;
        for (char c : password) charset.insert(c);
        double poolSize = charset.size();
        return password.length() * std::log2(poolSize);
    }

    bool isCommonPassword(const std::string& password,
                         const std::vector<std::string>& commonWords) {
        std::string lowerPass = toLower(password);
        return std::any_of(commonWords.begin(), commonWords.end(),
                          [&lowerPass](const std::string& word) {
                              return lowerPass.find(toLower(word)) != std::string::npos;
                          });
    }

    bool containsUpperCase(const std::string& str) {
        return std::any_of(str.begin(), str.end(),
                          [](char c) { return std::isupper(c); });
    }

    bool containsLowerCase(const std::string& str) {
        return std::any_of(str.begin(), str.end(),
                          [](char c) { return std::islower(c); });
    }

    bool containsDigit(const std::string& str) {
        return std::any_of(str.begin(), str.end(),
                          [](char c) { return std::isdigit(c); });
    }

    bool containsSpecialChar(const std::string& str) {
        return std::any_of(str.begin(), str.end(),
                          [](char c) { return std::ispunct(c); });
    }

    bool isSequential(const std::string& str) {
        if (str.length() < 3) return false;
        for (size_t i = 0; i < str.length() - 2; ++i) {
            if ((str[i + 1] == str[i] + 1 && str[i + 2] == str[i] + 2) ||
                (str[i + 1] == str[i] - 1 && str[i + 2] == str[i] - 2)) {
                return true;
            }
        }
        return false;
    }

    int getRandomInt(int min, int max) {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(min, max);
        return dis(gen);
    }

    char getRandomChar(const std::string& charset) {
        return charset[getRandomInt(0, charset.length() - 1)];
    }
}