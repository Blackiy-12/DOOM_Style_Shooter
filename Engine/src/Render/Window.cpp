#include "Window.h"

#include <Logger.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_vulkan.h>

Window::Window(std::string& WindowName, int Width, int Height)
{
	SDL_Init(SDL_INIT_VIDEO);

	this->WindowName = WindowName;

	WindowPtr = SDL_CreateWindow(this->WindowName.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, Width, Height, SDL_WINDOW_VULKAN);

	ENGINE_INFO("Window createad");
}

Window::~Window()
{
	SDL_DestroyWindow(WindowPtr);

	ENGINE_INFO("Window destroyed");
}
