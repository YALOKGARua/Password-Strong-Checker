#ifndef LOGGER_HPP
#define LOGGER_HPP
#include <string>
#include <fstream>
#include <mutex>
#include <memory>

enum class LogLevel {
    DEBUG,
    INFO,
    WARNING,
    ERROR,
    CRITICAL
};

class Logger {
public:
    explicit Logger(const std::string& filename);
    ~Logger();

    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;
    Logger(Logger&&) noexcept;
    Logger& operator=(Logger&&) noexcept;

    void log(const std::string& message, LogLevel level = LogLevel::INFO);
    void debug(const std::string& message);
    void info(const std::string& message);
    void warning(const std::string& message);
    void error(const std::string& message);
    void critical(const std::string& message);

    void setLogLevel(LogLevel level);
    LogLevel getLogLevel() const;
    void enableTimestamp(bool enable);
    void enableConsoleOutput(bool enable);
    void flush();

private:
    std::string filename_;
    std::ofstream file_;
    LogLevel min_level_;
    bool include_timestamp_;
    bool console_output_;
    std::mutex mutex_;

    std::string getCurrentTimestamp() const;
    std::string logLevelToString(LogLevel level) const;
    bool shouldLog(LogLevel level) const;
    void writeLog(const std::string& message, LogLevel level);
};

extern std::unique_ptr<Logger> g_logger;

#endif