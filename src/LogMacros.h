
#pragma once

#include <iostream>
#include <string>
#include <ctime>
#include <cstdarg>  // for va_list, va_start, va_end
#include <cstdio>   // for vsnprintf
#include <memory>   // for std::unique_ptr
#include <sstream>  // for std::stringstream
#include <vector>
#include <thread>   // for std::this_thread::get_id
#include <mutex>
#include <map>
#include <format>

// ANSI color codes
#define RESET         "\033[0m"
#define RED           "\033[31m"
#define GREEN         "\033[32m"
#define YELLOW        "\033[33m"
#define BLUE          "\033[34m"
#define MAGENTA       "\033[35m"
#define CYAN          "\033[36m"
#define WHITE         "\033[37m"

#define BOLD_WHITE    "\033[1;37m"  // Bold white
#define BOLD_YELLOW   "\033[1;33m"  // Bold yellow
#define BOLD_RED      "\033[1;31m"  // Bold red
#define DIM_WHITE     "\033[2;37m"

// Background colors
#define BG_BLACK      "\033[40m"
#define BG_RED        "\033[41m"
#define BG_GREEN      "\033[42m"
#define BG_YELLOW     "\033[43m"
#define BG_BLUE       "\033[44m"
#define BG_MAGENTA    "\033[45m"
#define BG_CYAN       "\033[46m"
#define BG_WHITE      "\033[47m"


// Max length of a log line before wrapping
const int LOG_LINE_LENGTH = 232; // You can adjust this value
const int TAB_SIZE = 4;

// Log levels in increasing order
enum class LogLevel {
    TRACE = 0,
    DEBUG,
    INFO,
    WARNING,
    ERROR,
    CRITICAL
};

// Function to convert LogLevel to string for display
inline const char* logLevelToString(LogLevel level) {
    switch (level) {
        case LogLevel::TRACE:    return "TRACE";
        case LogLevel::DEBUG:    return "DEBUG";
        case LogLevel::INFO:     return "INFO ";
        case LogLevel::WARNING:  return "WARN ";
        case LogLevel::ERROR:    return "ERROR";
        case LogLevel::CRITICAL: return "CRIT ";
        default:                 return "";
    }
}

// GroupLogger class to manage log levels for different groups
class PrettyLogger {
public:

    // Global instance of GroupLogger
    inline static PrettyLogger& Instance() {
        static PrettyLogger instance;
        return instance;
    }

    // Set the log level for a specific group
    void setGroupLogLevel(const std::string& group, LogLevel level) {
        std::lock_guard<std::mutex> lock(mutex_);
        groupLogLevels_[group] = level;
    }

    // Get the current log level for a specific group
    LogLevel getGroupLogLevel(const std::string& group) const {
        std::lock_guard<std::mutex> lock(mutex_);
        auto it = groupLogLevels_.find(group);
        return it != groupLogLevels_.end() ? it->second : defaultLogLevel_;
    }

    // Set the default log level (if no specific group level is set)
    void setDefaultLogLevel(LogLevel level) {
        std::lock_guard<std::mutex> lock(mutex_);
        defaultLogLevel_ = level;
    }

    // Check if a message should be logged for the specified group and level
    bool shouldLog(const std::string& group, LogLevel level) const {
        return level >= getGroupLogLevel(group);
    }

private:
    mutable std::mutex mutex_;
    LogLevel defaultLogLevel_ = LogLevel::TRACE;  // Default log level if none is set
    std::map<std::string, LogLevel> groupLogLevels_;
};

// Helper function to calculate how many spaces are needed for tab alignment
inline int calculateTabSpaces(int currentLength) {
	int nextTabStop = ((currentLength / TAB_SIZE) + 1) * TAB_SIZE;
	return nextTabStop - currentLength;
}

// Get the current timestamp as a string, with the year as the last two digits
inline std::string currentDateTime() {
    std::time_t now = std::time(nullptr);
    char buf[100];
    // Use %y to get the last two digits of the year
    std::strftime(buf, sizeof(buf), "%y%m%d:%H:%M:%S", std::localtime(&now));
    return std::string(buf);
}

template <typename... Args>
inline std::string formatString(std::string_view format_str, Args&&... args) {
    return std::vformat(format_str, std::make_format_args(std::forward<Args>(args)...));
}

// Function to wrap a string into lines of specified length
std::vector<std::string> wrapMessage(const std::string& message, int availableSpace);

// Function to extract the filename from the full file path
inline std::string getFileName(const std::string& filePath) {
	size_t pos = filePath.find_last_of("/\\");
	return (pos == std::string::npos) ? filePath : filePath.substr(pos + 1);
}

// Function to get the last 4 digits of the current thread ID as a string
inline std::string getThreadId() {
    std::ostringstream oss;
    oss << std::this_thread::get_id();
    std::string threadIdStr = oss.str();
    
    // Return the last 4 characters, or fewer if the thread ID is shorter
    return threadIdStr.size() > 4 ? threadIdStr.substr(threadIdStr.size() - 4) : threadIdStr;
}

// Base logging macro with file name and line number at the end, supporting printf-style formatting and colors
#define LOG_COLOR(level, color, bgColor, format, ...) \
	do { \
        if (PrettyLogger::Instance().shouldLog(LOG_GROUP, level)) { \
            std::string formattedMsg = formatString(format, ##__VA_ARGS__); \
            std::string fileInfo = getFileName(__FILE__) + ":" + std::to_string(__LINE__); \
            std::string dateTime = currentDateTime(); \
            int metadataLength = 27 + fileInfo.size() + 1;  /* timestamp + " | " + thread_id (max 19 chars) + " | " + log level + " | " */ \
            int availableSpace = LOG_LINE_LENGTH - metadataLength; \
            std::vector<std::string> wrappedLines = wrapMessage(formattedMsg, availableSpace); \
            std::cout << color << bgColor << dateTime << "|" << getThreadId() << "|" \
                << logLevelToString(level) << "| " << wrappedLines[0] << " " << fileInfo; \
            for (size_t i = 1; i < wrappedLines.size(); ++i) { \
                std::cout << "\n" << std::string(27 + 1, ' ') << wrappedLines[i]; \
            } \
            std::cout << RESET << std::endl; \
        } \
	} while (0)

#define LOG_TRACE(format, ...) LOG_COLOR(LogLevel::TRACE, DIM_WHITE, BG_BLACK, format, ##__VA_ARGS__)
#define LOG_INFO(format, ...) LOG_COLOR(LogLevel::INFO, GREEN, BG_BLACK, format, ##__VA_ARGS__)
#define LOG_DEBUG(format, ...) LOG_COLOR(LogLevel::DEBUG, CYAN, BG_BLACK, format, ##__VA_ARGS__)
#define LOG_WARNING(format, ...) LOG_COLOR(LogLevel::WARNING, BOLD_YELLOW, BG_BLACK, format, ##__VA_ARGS__)
#define LOG_ERROR(format, ...) LOG_COLOR(LogLevel::ERROR, BOLD_RED, BG_BLACK, format, ##__VA_ARGS__)
#define LOG_CRITICAL(format, ...) LOG_COLOR(LogLevel::CRITICAL, BOLD_WHITE, BG_RED, format, ##__VA_ARGS__)

#ifdef LOG_GROUP
    #undef LOG_GROUP
#endif
