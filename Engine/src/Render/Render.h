#pragma once

#include "../Core.h"
#include "Window.h"
#include "Device.h"
#include "SwapChain.h"

#include <memory>

class Render
{
public:
	Render();

	~Render();

	void init();

private:
	std::unique_ptr<Window> WindowPtr;

	std::unique_ptr<Device> DevicePtr;

	std::unique_ptr<SwapChain> SwapChainPtr;
};

