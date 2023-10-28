#include "Window.h"

#include <Logger.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_vulkan.h>

Window::Window(std::string& WindowName, int Height, int Width)
{
	SDL_Init(SDL_INIT_VIDEO);

	WindowPtr = SDL_CreateWindow(WindowName.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, Width, Height, SDL_WINDOW_VULKAN);

	ENGINE_INFO("Window createad");
}

Window::~Window()
{
	SDL_DestroyWindow(WindowPtr);

	ENGINE_INFO("Window destroyed");
}
