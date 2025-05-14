#include "ConfigManager.hpp"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <stdexcept>

ConfigManager::ConfigManager() {
    initializeDefaults();
}

ConfigManager::ConfigManager(const std::string& config_file) {
    initializeDefaults();
    if (!loadFromFile(config_file)) {
        throw std::runtime_error("Failed to load configuration from file: " + config_file);
    }
}

void ConfigManager::initializeDefaults() {
    min_length_ = 8;
    max_length_ = 128;
    strict_mode_ = false;
    min_entropy_bits_ = 50;
    
    common_words_ = {
        "password", "admin", "user", "login", "123456", "qwerty", "abc123",
        "letmein", "welcome", "monkey", "dragon", "baseball", "football",
        "superman", "batman", "trustno1", "sunshine", "princess", "freedom",
        "shadow", "master", "michael", "jennifer", "hunter", "buster",
        "thomas", "robert", "soccer", "hockey", "killer", "george",
        "charlie", "andrew", "michelle", "jordan", "taylor", "steven",
        "richard", "maggie", "pepper", "cheese", "david", "lucky",
        "flower", "angel", "tigger", "homer", "james", "johnny"
    };
}

size_t ConfigManager::getMinLength() const {
    return min_length_;
}

size_t ConfigManager::getMaxLength() const {
    return max_length_;
}

const std::vector<std::string>& ConfigManager::getCommonWords() const {
    return common_words_;
}

bool ConfigManager::isStrictMode() const {
    return strict_mode_;
}

int ConfigManager::getMinEntropyBits() const {
    return min_entropy_bits_;
}

void ConfigManager::setMinLength(size_t length) {
    if (length > max_length_) throw std::invalid_argument("Minimum length cannot be greater than maximum length");
    min_length_ = length;
}

void ConfigManager::setMaxLength(size_t length) {
    if (length < min_length_) throw std::invalid_argument("Maximum length cannot be less than minimum length");
    max_length_ = length;
}

void ConfigManager::setStrictMode(bool strict) {
    strict_mode_ = strict;
}

void ConfigManager::setMinEntropyBits(int bits) {
    if (bits < 0) throw std::invalid_argument("Minimum entropy bits cannot be negative");
    min_entropy_bits_ = bits;
}

void ConfigManager::addCommonWord(const std::string& word) {
    if (std::find(common_words_.begin(), common_words_.end(), word) == common_words_.end()) {
        common_words_.push_back(word);
    }
}

void ConfigManager::removeCommonWord(const std::string& word) {
    common_words_.erase(
        std::remove(common_words_.begin(), common_words_.end(), word),
        common_words_.end()
    );
}

bool ConfigManager::loadFromFile(const std::string& filename) {
    try {
        std::ifstream file(filename);
        if (!file.is_open()) return false;

        std::string line;
        while (std::getline(file, line)) {
            if (line.empty() || line[0] == '#') continue;
            
            std::istringstream iss(line);
            std::string key;
            if (std::getline(iss, key, '=')) {
                std::string value;
                if (std::getline(iss, value)) {
                    if (key == "min_length") setMinLength(std::stoi(value));
                    else if (key == "max_length") setMaxLength(std::stoi(value));
                    else if (key == "strict_mode") setStrictMode(value == "true" || value == "1");
                    else if (key == "min_entropy_bits") setMinEntropyBits(std::stoi(value));
                    else if (key == "common_word") addCommonWord(value);
                }
            }
        }
        return true;
    }
    catch (const std::exception& e) {
        return false;
    }
}

bool ConfigManager::saveToFile(const std::string& filename) const {
    try {
        std::ofstream file(filename);
        if (!file.is_open()) return false;

        file << "min_length=" << min_length_ << "\n";
        file << "max_length=" << max_length_ << "\n";
        file << "strict_mode=" << (strict_mode_ ? "true" : "false") << "\n";
        file << "min_entropy_bits=" << min_entropy_bits_ << "\n";

        for (const auto& word : common_words_) {
            file << "common_word=" << word << "\n";
        }
        
        return true;
    }
    catch (const std::exception& e) {
        return false;
    }
}

void ConfigManager::resetToDefaults() {
    initializeDefaults();
}