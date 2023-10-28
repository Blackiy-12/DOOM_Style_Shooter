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
}
