#include"App.h"

#include <Logger.h>

extern Application* createApplication();

int main(int argc, char** argv)
{
	Log::Logger::init();

	auto app = createApplication();

	app->run();

	delete app;


	return 0;
}