


#include <iostream>
#include <cstdlib>

#include "MainApp.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "LogMacros.h"
#ifdef LOG_GROUP
	#undef LOG_GROUP 
#endif
#define LOG_GROUP LOG_GROUP_DEFAULT
#include <iostream>
#include <stdexcept>
#include <execinfo.h>
#include <cstdlib>
#include <cxxabi.h>

void printStackTrace() {
    const int maxFrames = 100;
    void* callStack[maxFrames];
    int frameCount = backtrace(callStack, maxFrames);
    char** symbols = backtrace_symbols(callStack, frameCount);

	std::stringstream ss;

    ss << "Stack Trace:\n";

    for (int i = 0; i < frameCount; ++i) {
        // Demangle the symbol name (if available)
        char* mangledName = nullptr;
        char* offsetBegin = nullptr;
        char* offsetEnd = nullptr;

        for (char* p = symbols[i]; *p; ++p) {
            if (*p == '(') {
                mangledName = p;
            } else if (*p == '+') {
                offsetBegin = p;
            } else if (*p == ')') {
                offsetEnd = p;
                break;
            }
        }

        if (mangledName && offsetBegin && offsetEnd && mangledName < offsetBegin) {
            *mangledName++ = '\0';
            *offsetBegin++ = '\0';
            *offsetEnd = '\0';

            int status;
            char* realName = abi::__cxa_demangle(mangledName, nullptr, nullptr, &status);

            if (status == 0) {
                ss << "    " << symbols[i] << " : " << realName << "+" << offsetBegin << "\n";
            } else {
                ss << "    " << symbols[i] << " : " << mangledName << "+" << offsetBegin << "\n";
            }
            free(realName);
        } else {
            ss << "    " << symbols[i] << "\n";
        }
    }

	LOG_CRITICAL("{}", ss.str());
    free(symbols);
}

void exceptionHandler() {
    try {
        throw;  // Re-throw the current exception
    } catch (const std::exception& e) {
		LOG_CRITICAL("exception: {}", e.what());
        printStackTrace();
    } catch (...) {
		LOG_CRITICAL("Unknown exception caught!");
        printStackTrace();
    }
}

int main() 
{

	// TODO: do a cfg load for log levels

    PrettyLogger::Instance().setGroupLogLevel(LOG_GROUP_DEFAULT, LogLevel::TRACE);
    PrettyLogger::Instance().setGroupLogLevel(LOG_GROUP_VULKAN, LogLevel::TRACE);

	try {
		MainApp::Instance();
	} catch (const std::exception& e) {
		exceptionHandler();
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}