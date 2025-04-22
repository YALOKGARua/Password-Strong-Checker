#include "PasswordChecker.hpp"
#include "Utils.hpp"
#include <algorithm>
#include <cmath>
#include <set>
#include <stdexcept>

PasswordChecker::PasswordChecker(const ConfigManager& config) : config_(config) {}

PasswordStrength PasswordChecker::checkPassword(const std::string& password) {
    last_check_details_.clear();

    if (password.empty()) {
        last_check_details_ = "Password cannot be empty";
        throw std::invalid_argument(last_check_details_);
    }

    bool length_ok = checkLength(password);
    bool uppercase_ok = checkUpperCase(password);
    bool lowercase_ok = checkLowerCase(password);
    bool digits_ok = checkDigits(password);
    bool special_ok = checkSpecialChars(password);
    bool no_repeating_ok = checkNoRepeatingChars(password);
    bool no_sequences_ok = checkNoSequences(password);
    bool no_common_words_ok = checkNoCommonWords(password);
    double entropy = calculateEntropy(password);

    last_check_details_ += "Password Analysis:\n";
    last_check_details_ += "- Length: " + std::string(length_ok ? "OK" : "Insufficient") + "\n";
    last_check_details_ += "- Uppercase Letters: " + std::string(uppercase_ok ? "OK" : "Missing") + "\n";
    last_check_details_ += "- Lowercase Letters: " + std::string(lowercase_ok ? "OK" : "Missing") + "\n";
    last_check_details_ += "- Digits: " + std::string(digits_ok ? "OK" : "Missing") + "\n";
    last_check_details_ += "- Special Characters: " + std::string(special_ok ? "OK" : "Missing") + "\n";
    last_check_details_ += "- No Repeating Characters: " + std::string(no_repeating_ok ? "OK" : "Has Repeats") + "\n";
    last_check_details_ += "- No Sequences: " + std::string(no_sequences_ok ? "OK" : "Has Sequences") + "\n";
    last_check_details_ += "- No Common Words: " + std::string(no_common_words_ok ? "OK" : "Contains Common Words") + "\n";
    last_check_details_ += "- Entropy: " + std::to_string(entropy) + " bits\n";

    return evaluateStrength(password);
}

bool PasswordChecker::checkLength(const std::string& password) const {
    return password.length() >= config_.getMinLength();
}

bool PasswordChecker::checkUpperCase(const std::string& password) const {
    return std::any_of(password.begin(), password.end(), [](char c) { return std::isupper(c); });
}

bool PasswordChecker::checkLowerCase(const std::string& password) const {
    return std::any_of(password.begin(), password.end(), [](char c) { return std::islower(c); });
}

bool PasswordChecker::checkDigits(const std::string& password) const {
    return std::any_of(password.begin(), password.end(), [](char c) { return std::isdigit(c); });
}

bool PasswordChecker::checkSpecialChars(const std::string& password) const {
    return std::any_of(password.begin(), password.end(), [](char c) {
        return std::ispunct(c);
        });
}

bool PasswordChecker::checkNoRepeatingChars(const std::string& password) const {
    std::set<char> chars(password.begin(), password.end());
    return chars.size() == password.length();
}

bool PasswordChecker::checkNoSequences(const std::string& password) const {
    for (size_t i = 0; i < password.length() - 2; ++i) {
        if (std::isalpha(password[i]) && std::isalpha(password[i + 1]) && std::isalpha(password[i + 2])) {
            if (password[i + 1] == password[i] + 1 && password[i + 2] == password[i] + 2) {
                return false;
            }
        }
        if (std::isdigit(password[i]) && std::isdigit(password[i + 1]) && std::isdigit(password[i + 2])) {
            if (password[i + 1] == password[i] + 1 && password[i + 2] == password[i] + 2) {
                return false;
            }
        }
    }
    return true;
}

bool PasswordChecker::checkNoCommonWords(const std::string& password) const {
    std::string lower_password = Utils::toLower(password);
    for (const auto& word : config_.getCommonWords()) {
        if (lower_password.find(word) != std::string::npos) {
            return false;
        }
    }
    return true;
}

double PasswordChecker::calculateEntropy(const std::string& password) const {
    std::set<char> unique_chars(password.begin(), password.end());
    double charset_size = unique_chars.size();
    return password.length() * std::log2(charset_size);
}

PasswordStrength PasswordChecker::evaluateStrength(const std::string& password) {
    int score = 0;
    if (checkLength(password)) score += 20;
    if (checkUpperCase(password)) score += 15;
    if (checkLowerCase(password)) score += 15;
    if (checkDigits(password)) score += 15;
    if (checkSpecialChars(password)) score += 15;
    if (checkNoRepeatingChars(password)) score += 10;
    if (checkNoSequences(password)) score += 10;
    if (checkNoCommonWords(password)) score += 10;
    double entropy = calculateEntropy(password);
    if (entropy > 50) score += 20;
    else if (entropy > 30) score += 10;

    if (score >= 90) return PasswordStrength::VERY_STRONG;
    if (score >= 70) return PasswordStrength::STRONG;
    if (score >= 50) return PasswordStrength::MEDIUM;
    return PasswordStrength::WEAK;
}

std::string PasswordChecker::strengthToString(PasswordStrength strength) const {
    switch (strength) {
    case PasswordStrength::WEAK: return "Weak";
    case PasswordStrength::MEDIUM: return "Medium";
    case PasswordStrength::STRONG: return "Strong";
    case PasswordStrength::VERY_STRONG: return "Very Strong";
    default: return "Unknown";
    }
}

std::string PasswordChecker::getLastCheckDetails() const {
    return last_check_details_;
}
