#pragma once

#ifdef SHOOTER_BUILD_DLL
	#define SHOOTER_API __declspec(dllexport)
#else
	#define SHOOTER_API __declspec(dllimport)
#endif