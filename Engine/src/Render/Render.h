#pragma once

#include "../Core.h"
#include "Window.h"


#include <memory>

class Render
{
public:
	Render();

	~Render();

	void init();

private:
	std::unique_ptr<Window> WindowPtr;

};
