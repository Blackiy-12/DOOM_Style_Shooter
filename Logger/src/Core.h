#pragma once

#ifdef LOG_BUILD_DLL
	#define LOG_API __declspec(dllexport)
#else
	#define LOG_API __declspec(dllimport)
#endif