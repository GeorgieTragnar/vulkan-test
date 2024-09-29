
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

// Function to deduce the format specifier based on the argument type
template <typename T>
std::string deduceFormatSpecifier(const T& arg) {
    if constexpr (std::is_integral<T>::value && !std::is_same<T, char>::value) {
        return "%d";  // Use %d for integral types (except char)
    } else if constexpr (std::is_floating_point<T>::value) {
        return "%f";  // Use %f for floating-point types
    } else if constexpr (std::is_same<T, const char*>::value || std::is_same<T, std::string>::value) {
        return "%s";  // Use %s for strings
    } else if constexpr (std::is_array<T>::value && std::is_same<std::remove_extent_t<T>, char>::value) {
        return "%s";  // Treat char arrays (like char[256]) as %s for strings
    } else if constexpr (std::is_same<T, char>::value) {
        return "%c";  // Use %c for single characters
    } else {
        return "%x";  // For unsupported types
    }
}
// Function that substitutes '{}' with printf-style specifiers based on the arguments
template <typename... Args>
std::string substituteFormatSpecifiers(const std::string& formatStr, const Args&... args) {
    std::ostringstream oss;
    const char* currentChar = formatStr.c_str();
    size_t argIndex = 0;

    // Create a tuple from the variadic arguments
    std::tuple<const Args&...> arguments(args...);

    while (*currentChar) {
        if (*currentChar == '{' && *(currentChar + 1) == '}') {
            // Substitute with the appropriate format specifier
            oss << std::apply(
                [&argIndex](const auto&... args) {
                    return std::array<std::string, sizeof...(args)>{
                        deduceFormatSpecifier(args)...}[argIndex++];
                },
                arguments);
            currentChar += 2;  // Move past the '{}'
        } else {
            // Copy the character as-is
            oss << *currentChar;
            ++currentChar;
        }
    }

    return oss.str();
}

// Get the current timestamp as a string, with the year as the last two digits
inline std::string currentDateTime() {
    std::time_t now = std::time(nullptr);
    char buf[100];
    // Use %y to get the last two digits of the year
    std::strftime(buf, sizeof(buf), "%y%m%d:%H:%M:%S", std::localtime(&now));
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
		std::string subFormat = substituteFormatSpecifiers(format, ##__VA_ARGS__); \
		std::string formattedMsg = formatString(subFormat.c_str(), ##__VA_ARGS__); \
		std::string fileInfo = getFileName(__FILE__) + ":" + std::to_string(__LINE__); \
		std::string dateTime = currentDateTime(); \
		int metadataLength = 27 + fileInfo.size() + 1;  /* timestamp + " | " + thread_id (max 19 chars) + " | " + log level + " | " */ \
		int availableSpace = LOG_LINE_LENGTH - metadataLength; \
		std::vector<std::string> wrappedLines = wrapMessage(formattedMsg, availableSpace); \
		std::cout << color << bgColor << dateTime << "|" << getThreadId() << "|" \
			<< level << "| " << wrappedLines[0] << " " << fileInfo; \
		for (size_t i = 1; i < wrappedLines.size(); ++i) { \
			std::cout << "\n" << std::string(27 + 1, ' ') << wrappedLines[i]; \
		} \
		std::cout << RESET << std::endl; \
	} while (0)

#define LOG_TRACE(format, ...) LOG_COLOR("TRACE", DIM_WHITE, BG_BLACK, format, ##__VA_ARGS__)
#define LOG_INFO(format, ...) LOG_COLOR("INFO ", GREEN, BG_BLACK, format, ##__VA_ARGS__)
#define LOG_DEBUG(format, ...) LOG_COLOR("DEBUG", CYAN, BG_BLACK, format, ##__VA_ARGS__)
#define LOG_WARNING(format, ...) LOG_COLOR("WARN ", BOLD_YELLOW, BG_BLACK, format, ##__VA_ARGS__)
#define LOG_ERROR(format, ...) LOG_COLOR("ERROR", BOLD_RED, BG_BLACK, format, ##__VA_ARGS__)
#define LOG_CRITICAL(format, ...) LOG_COLOR("CRIT ", BOLD_WHITE, BG_RED, format, ##__VA_ARGS__)
