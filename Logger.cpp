#include "Logger.hpp"
#include <ctime>

Logger::Logger(const std::string& filename) {
    file_.open(filename, std::ios::app);
}

Logger::~Logger() {
    if (file_.is_open()) {
        file_.close();
    }
}

void Logger::log(const std::string& message) {
    if (file_.is_open()) {
        std::time_t now = std::time(nullptr);
        char buffer[26];
        ctime_s(buffer, sizeof(buffer), &now);
        file_ << buffer << ": " << message << "\n";
        file_.flush();
    }
}