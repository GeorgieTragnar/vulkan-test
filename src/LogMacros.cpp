
#include "LogMacros.h"


// Function to wrap a string into lines of specified length
std::vector<std::string> wrapString(const std::string& str, int lineLength) {
    std::vector<std::string> lines;
    std::istringstream stream(str);
    std::string word;
    std::string currentLine;

    while (stream >> word) {
        if (currentLine.size() + word.size() + 1 > lineLength) {
            lines.push_back(currentLine);
            currentLine = word;  // Start a new line with the current word
        } else {
            if (!currentLine.empty()) {
                currentLine += " ";
            }
            currentLine += word;
        }
    }

    if (!currentLine.empty()) {
        lines.push_back(currentLine);  // Push the last line
    }

    return lines;
}
