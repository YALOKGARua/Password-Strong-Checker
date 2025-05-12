#include "Logger.hpp"
#include <iostream>
#include <ctime>
#include <iomanip>
#include <sstream>

std::unique_ptr<Logger> g_logger;

Logger::Logger(const std::string& filename)
    : filename_(filename),
    min_level_(LogLevel::INFO),
    include_timestamp_(true),
    console_output_(false) {
    file_.open(filename, std::ios::app);
    if (!file_.is_open()) throw std::runtime_error("Failed to open log file: " + filename);
}

Logger::~Logger() {
    if (file_.is_open()) file_.close();
}

Logger::Logger(Logger&& other) noexcept
    : filename_(std::move(other.filename_)),
    file_(std::move(other.file_)),
    min_level_(other.min_level_),
    include_timestamp_(other.include_timestamp_),
    console_output_(other.console_output_) {
}

Logger& Logger::operator=(Logger&& other) noexcept {
    if (this != &other) {
        if (file_.is_open()) file_.close();
        filename_ = std::move(other.filename_);
        file_ = std::move(other.file_);
        min_level_ = other.min_level_;
        include_timestamp_ = other.include_timestamp_;
        console_output_ = other.console_output_;
    }
    return *this;
}

void Logger::log(const std::string& message, LogLevel level) {
    if (shouldLog(level)) writeLog(message, level);
}

void Logger::debug(const std::string& message) {
    log(message, LogLevel::DEBUG);
}

void Logger::info(const std::string& message) {
    log(message, LogLevel::INFO);
}

void Logger::warning(const std::string& message) {
    log(message, LogLevel::WARNING);
}

void Logger::error(const std::string& message) {
    log(message, LogLevel::ERROR);
}

void Logger::critical(const std::string& message) {
    log(message, LogLevel::CRITICAL);
}

void Logger::setLogLevel(LogLevel level) {
    min_level_ = level;
}

LogLevel Logger::getLogLevel() const {
    return min_level_;
}

void Logger::enableTimestamp(bool enable) {
    include_timestamp_ = enable;
}

void Logger::enableConsoleOutput(bool enable) {
    console_output_ = enable;
}

void Logger::flush() {
    std::lock_guard<std::mutex> lock(mutex_);
    file_.flush();
}

std::string Logger::getCurrentTimestamp() const {
    auto now = std::chrono::system_clock::now();
    auto time = std::chrono::system_clock::to_time_t(now);
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
        now.time_since_epoch()
    ).count() % 1000;

    std::stringstream ss;
    ss << std::put_time(std::localtime(&time), "%Y-%m-%d %H:%M:%S");
    ss << '.' << std::setfill('0') << std::setw(3) << ms;
    return ss.str();
}

std::string Logger::logLevelToString(LogLevel level) const {
    switch (level) {
        case LogLevel::DEBUG: return "DEBUG";
        case LogLevel::INFO: return "INFO";
        case LogLevel::WARNING: return "WARNING";
        case LogLevel::ERROR: return "ERROR";
        case LogLevel::CRITICAL: return "CRITICAL";
        default: return "UNKNOWN";
    }
}

bool Logger::shouldLog(LogLevel level) const {
    return level >= min_level_;
}

void Logger::writeLog(const std::string& message, LogLevel level) {
    std::lock_guard<std::mutex> lock(mutex_);
    
    std::stringstream log_stream;
    if (include_timestamp_) log_stream << "[" << getCurrentTimestamp() << "] ";
    log_stream << "[" << logLevelToString(level) << "] " << message << "\n";
    
    if (file_.is_open()) {
        file_ << log_stream.str();
        file_.flush();
    }
    
    if (console_output_) {
        std::cout << log_stream.str();
        std::cout.flush();
    }
}