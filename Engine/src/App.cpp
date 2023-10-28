#include "App.h"

Application* Application::AppPtr = nullptr;

Application::Application()
{
	AppPtr = this;

	this->EnginePtr = std::make_unique<Engine>();
}

void Application::run()
{
	while (true)
	{

	}
}
