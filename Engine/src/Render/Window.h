#pragma once

#include "../Core.h"

#include <string>

struct SDL_Window;

class Device;

class ENGINE_API Window
{
public:
	Window(std::string& WindowName, int Width = 800, int Height = 600);

	~Window();

public:
	std::string getWindowName() { return WindowName; }

private:

	SDL_Window* WindowPtr;

	std::string WindowName;


private:
	friend class Device;
};

