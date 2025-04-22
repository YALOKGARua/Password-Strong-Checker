#ifndef CONFIG_MANAGER_HPP
#define CONFIG_MANAGER_HPP
#include <vector>
#include <string>

class ConfigManager {
public:
    ConfigManager();
    size_t getMinLength() const;
    const std::vector<std::string>& getCommonWords() const;

private:
    size_t min_length_;
    std::vector<std::string> common_words_;
};

#endif
