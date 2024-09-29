


#include <iostream>
#include <cstdlib>

#include "MainApp.h"

#include "LogMacros.h"
#ifdef LOG_GROUP
	#undef LOG_GROUP 
#endif
#define LOG_GROUP LOG_GROUP_DEFAULT

int main() 
{
	MainApp app;

	// TODO: do a cfg load for log levels

    PrettyLogger::Instance().setGroupLogLevel(LOG_GROUP_DEFAULT, LogLevel::TRACE);
    PrettyLogger::Instance().setGroupLogLevel(LOG_GROUP_VULKAN, LogLevel::TRACE);

	try {
		app.run();
	} catch (const std::exception& e) {
		LOG_CRITICAL("exception: {}", e.what());
		// std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}