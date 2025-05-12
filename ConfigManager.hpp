#ifndef CONFIG_MANAGER_HPP
#define CONFIG_MANAGER_HPP
#include <vector>
#include <string>
#include <map>

class ConfigManager {
public:
    ConfigManager();
    explicit ConfigManager(const std::string& config_file);
    
    size_t getMinLength() const;
    size_t getMaxLength() const;
    const std::vector<std::string>& getCommonWords() const;
    bool isStrictMode() const;
    int getMinEntropyBits() const;
    
    void setMinLength(size_t length);
    void setMaxLength(size_t length);
    void setStrictMode(bool strict);
    void setMinEntropyBits(int bits);
    void addCommonWord(const std::string& word);
    void removeCommonWord(const std::string& word);
    
    bool loadFromFile(const std::string& filename);
    bool saveToFile(const std::string& filename) const;
    void resetToDefaults();

private:
    size_t min_length_;
    size_t max_length_;
    bool strict_mode_;
    int min_entropy_bits_;
    std::vector<std::string> common_words_;
    std::map<std::string, std::string> custom_rules_;
    void initializeDefaults();
};

#endif