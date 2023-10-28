#pragma once

#include "../Core.h"

#include <string>

struct SDL_Window;

class ENGINE_API Window
{
public:
	Window(std::string& WindowName, int Width = 800, int Height = 600);

	~Window();

private:

	SDL_Window* WindowPtr;

};

