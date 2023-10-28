#pragma once

#include "Core.h"
#include "Render/Render.h"

#include <memory>

class ENGINE_API Engine
{
public:
	Engine();
	
	~Engine();

private:
	std::unique_ptr<Render> RenderPtr;
};

