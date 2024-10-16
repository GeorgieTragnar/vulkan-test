
#include "MainApp.h"

#include "LogMacros.h"
#ifdef LOG_GROUP
	#undef LOG_GROUP 
#endif
#define LOG_GROUP LOG_GROUP_DEFAULT

MainApp::MainApp()
{
	run();
}

MainApp::~MainApp()
{
	LOG_INFO("MainApp cleanup");
}

void MainApp::run() 
{
	Window::Instance();
	VulkanApp::Instance();
	Window::Instance().setFrameBufferResizeCallback(VulkanApp::Instance().getResizedCallback());
	VideoStream::Instance();

	std::thread([] {start_libdatachannel_streamer();}).detach();
	

	mainLoop();
}

void MainApp::mainLoop() 
{
	using clock = std::chrono::high_resolution_clock;
	auto startTime = clock::now();
	int frameCount = 0;
	const int frameLimit = 50000; // Measure after 50000 frames

	while (Window::Instance().loopStep())
	{
		VulkanApp::Instance().drawFrame();
		frameCount++;

		if (frameCount == frameLimit) {
			auto endTime = clock::now();
			auto duration = std::chrono::duration_cast<std::chrono::seconds>(endTime - startTime).count();

			if (duration > 0) {
				float averageFps = frameLimit / static_cast<float>(duration);
				LOG_TRACE("Average FPS over {} frames: {}", frameLimit, averageFps);
			}

			// Reset frame counter and timer
			frameCount = 0;
			startTime = clock::now();
		}
	}

	VulkanApp::Instance().waitIdle();
}
