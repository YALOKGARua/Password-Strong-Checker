#ifndef PASSWORD_CHECKER_HPP
#define PASSWORD_CHECKER_HPP
#include <string>
#include <vector>
#include "ConfigManager.hpp"

enum class PasswordStrength {
    WEAK,
    MEDIUM,
    STRONG,
    VERY_STRONG
};

class PasswordChecker {
public:
    explicit PasswordChecker(const ConfigManager& config);
    PasswordStrength checkPassword(const std::string& password);
    std::string strengthToString(PasswordStrength strength) const;
    std::string getLastCheckDetails() const;

private:
    const ConfigManager& config_;
    std::string last_check_details_;

    bool checkLength(const std::string& password) const;
    bool checkUpperCase(const std::string& password) const;
    bool checkLowerCase(const std::string& password) const;
    bool checkDigits(const std::string& password) const;
    bool checkSpecialChars(const std::string& password) const;
    bool checkNoRepeatingChars(const std::string& password) const;
    bool checkNoSequences(const std::string& password) const;
    bool checkNoCommonWords(const std::string& password) const;
    double calculateEntropy(const std::string& password) const;
    PasswordStrength evaluateStrength(const std::string& password);
};

#endif