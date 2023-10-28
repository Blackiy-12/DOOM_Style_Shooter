#include "Render.h"

Render::Render()
{
	init();
}

Render::~Render()
{
}

void Render::init()
{
	std::string Name = "VulkanWindow";

	WindowPtr = std::make_unique<Window>(Name);

	DevicePtr = std::make_unique<Device>(WindowPtr.get());

	SwapChainPtr = std::make_unique<SwapChain>( DevicePtr.get(), WindowPtr.get());
}
