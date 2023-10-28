#include "Engine.h"

Engine::Engine()
{
	RenderPtr = std::make_unique<Render>();
}

Engine::~Engine()
{
}
