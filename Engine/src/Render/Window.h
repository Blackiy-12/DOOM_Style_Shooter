#pragma once

#include "../Core.h"

#include <vulkan/vulkan.h>

#include <string>

struct SDL_Window;

class Device;

class SwapChain;

class ENGINE_API Window
{
public:
	Window(std::string& WindowName, int Width = 800, int Height = 600);

	~Window();

public:
	std::string getWindowName() { return WindowName; };

	void createSurface(VkSurfaceKHR* WindowSurface, VkInstance Instance);

private:

	SDL_Window* WindowPtr;

	std::string WindowName;

private:
	friend class Device;
	friend class SwapChain;
};

