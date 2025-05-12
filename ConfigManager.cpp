#include "ConfigManager.hpp"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <stdexcept>
#include <json/json.h>

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

        Json::Value root;
        file >> root;

        if (root.isMember("min_length")) setMinLength(root["min_length"].asUInt());
        if (root.isMember("max_length")) setMaxLength(root["max_length"].asUInt());
        if (root.isMember("strict_mode")) setStrictMode(root["strict_mode"].asBool());
        if (root.isMember("min_entropy_bits")) setMinEntropyBits(root["min_entropy_bits"].asInt());
        
        if (root.isMember("common_words")) {
            common_words_.clear();
            const Json::Value& words = root["common_words"];
            for (const auto& word : words) common_words_.push_back(word.asString());
        }

        return true;
    }
    catch (const std::exception& e) {
        return false;
    }
}

bool ConfigManager::saveToFile(const std::string& filename) const {
    try {
        Json::Value root;
        root["min_length"] = static_cast<Json::UInt>(min_length_);
        root["max_length"] = static_cast<Json::UInt>(max_length_);
        root["strict_mode"] = strict_mode_;
        root["min_entropy_bits"] = min_entropy_bits_;

        Json::Value words(Json::arrayValue);
        for (const auto& word : common_words_) words.append(word);
        root["common_words"] = words;

        std::ofstream file(filename);
        if (!file.is_open()) return false;

        Json::StyledWriter writer;
        file << writer.write(root);
        return true;
    }
    catch (const std::exception& e) {
        return false;
    }
}

void ConfigManager::resetToDefaults() {
    initializeDefaults();
}