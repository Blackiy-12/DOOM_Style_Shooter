#pragma once

#include "Core.h"
#include "Engine.h"

int main(int argc, char** argv);

class ENGINE_API Application
{
public:
	Application();
	
	virtual ~Application() {};

private:
	void run();

private:
	std::unique_ptr<Engine> EnginePtr;

private:
	static Application* AppPtr;

	friend int main(int argc, char** argv);
};

Application* createApplication();

