
#include "LogMacros.h"



// Helper function to calculate how many spaces are needed for tab alignment
int calculateTabSpaces(int currentLength) {
	int nextTabStop = ((currentLength / TAB_SIZE) + 1) * TAB_SIZE;
	return nextTabStop - currentLength;
}

// Function to split a single string into a vector of strings of fixed size (availableSpace)
std::vector<std::string> wrapMessage(const std::string& message, int availableSpace) {
	std::vector<std::string> lines;
	std::string currentLine;
	std::istringstream stream(message);
	std::string word;
	char ch;

	while (stream.get(ch)) {
		if (ch == '\t') {
			// Handle tab (\t) by adding the necessary number of spaces
			int tabSpaces = calculateTabSpaces(currentLine.size());
			currentLine += std::string(tabSpaces, ' ');
		} else if (std::isspace(ch)) {
			// Add spaces to the line, preserving spaces (but split at availableSpace)
			if (currentLine.size() + 1 > availableSpace) {
				lines.push_back(currentLine);
				currentLine = "";  // Start new line
			}
			currentLine += ch;  // Add space character
		} else {
			// Gather the word (any sequence of non-space characters)
			word.clear();
			word += ch;
			while (stream.peek() != EOF && !std::isspace(stream.peek())) {
				word += stream.get();
			}
			
			// Handle word splitting if it's longer than availableSpace
			while (!word.empty()) {
				if (currentLine.size() + word.size() <= availableSpace) {
					// If word fits in the current line, append it
					currentLine += word;
					word.clear();  // Word has been fully processed
				} else {
					// If word doesn't fit, split it
					int spaceLeft = availableSpace - currentLine.size();
					currentLine += word.substr(0, spaceLeft);
					lines.push_back(currentLine);
					currentLine = "";  // Start a new line
					word = word.substr(spaceLeft);  // Remaining part of the word
				}
			}
		}
	}
	
	// Add the last line if it's not empty and pad it if necessary
	if (!currentLine.empty()) {
		// If word doesn't fit, split it
		int spaceLeft = availableSpace - currentLine.size();
		currentLine += std::string(spaceLeft, ' ');
				// std::cout << "\n3 " << currentLine.size() << " " << availableSpace << "\n";
		lines.push_back(currentLine);
	}

	return lines;
}
