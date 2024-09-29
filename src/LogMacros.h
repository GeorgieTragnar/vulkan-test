
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

// ANSI color codes
#define RESET   "\033[0m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"
#define WHITE   "\033[37m"

// Max length of a log line before wrapping
const int LOG_LINE_LENGTH = 80; // You can adjust this value

// Get the current timestamp as a string
inline std::string currentDateTime() {
    std::time_t now = std::time(nullptr);
    char buf[100];
    std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", std::localtime(&now));
    return std::string(buf);
}

// Helper function for printf-style formatting
inline std::string formatString(const char* format, ...) {
    va_list args;
    va_start(args, format);

    // First, copy the va_list and calculate the required buffer size
    va_list args_copy;
    va_copy(args_copy, args);
    size_t size = std::vsnprintf(nullptr, 0, format, args_copy) + 1;  // +1 for null terminator
    va_end(args_copy);

    // Now, format the string with the correct buffer size
    std::unique_ptr<char[]> buf(new char[size]);
    std::vsnprintf(buf.get(), size, format, args);
    va_end(args);

    return std::string(buf.get(), buf.get() + size - 1);  // Exclude null terminator
}

// Function to wrap a string into lines of specified length
std::vector<std::string> wrapString(const std::string& str, int lineLength);

// Function to get the current thread ID as a string
inline std::string getThreadId() {
    std::ostringstream oss;
    oss << std::this_thread::get_id();
    return oss.str();
}

// Base logging macro with file name and line number at the end, supporting printf-style formatting and colors
#define LOG_COLOR(level, color, format, ...) \
    do { \
        std::string formattedMsg = formatString(format, ##__VA_ARGS__); \
        std::vector<std::string> wrappedLines = wrapString(formattedMsg, LOG_LINE_LENGTH); \
        for (size_t i = 0; i < wrappedLines.size(); ++i) { \
            std::cout << color << currentDateTime() << " | " << getThreadId() << " | " \
                      << level << " | " << wrappedLines[i]; \
            if (i == wrappedLines.size() - 1) { \
                std::cout << " | " << __FILE__ << ":" << __LINE__; \
            } \
            std::cout << RESET << std::endl; \
        } \
    } while (0)

// Info log with green color
#define LOG_INFO(format, ...) LOG_COLOR("INFO", GREEN, format, ##__VA_ARGS__)

// Warning log with yellow color
#define LOG_WARNING(format, ...) LOG_COLOR("WARNING", YELLOW, format, ##__VA_ARGS__)

// Error log with red color
#define LOG_ERROR(format, ...) LOG_COLOR("ERROR", RED, format, ##__VA_ARGS__)

// Debug log with cyan color
#define LOG_DEBUG(format, ...) LOG_COLOR("DEBUG", CYAN, format, ##__VA_ARGS__)

// Critical log with magenta color
#define LOG_CRITICAL(format, ...) LOG_COLOR("CRITICAL", MAGENTA, format, ##__VA_ARGS__)
