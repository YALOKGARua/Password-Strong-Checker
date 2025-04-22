#ifndef LOGGER_HPP
#define LOGGER_HPP
#include <string>
#include <fstream>

class Logger {
public:
    explicit Logger(const std::string& filename);
    ~Logger();
    void log(const std::string& message);

private:
    std::ofstream file_;
};

#endif