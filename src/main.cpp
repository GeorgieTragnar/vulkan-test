


#include <iostream>
#include <cstdlib>

#include "MainApp.h"


int main() 
{
	MainApp app;

	try {
		app.run();
	} catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}