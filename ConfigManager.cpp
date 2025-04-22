#include "ConfigManager.hpp"

ConfigManager::ConfigManager() : min_length_(8) {
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

const std::vector<std::string>& ConfigManager::getCommonWords() const {
    return common_words_;
}