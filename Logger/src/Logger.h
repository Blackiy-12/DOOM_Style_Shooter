#pragma once

#include "Core.h"

#include <spdlog/spdlog.h>
#include <memory>

namespace Log
{
	class LOG_API Logger
	{
	public:
		static void init();

		inline static std::shared_ptr<spdlog::logger> getLoggerEngine() { return LogEngine; };

		inline static std::shared_ptr<spdlog::logger> getLoggerShooter() { return LogShooter; };

	private:
		static std::shared_ptr<spdlog::logger> LogEngine;
		static std::shared_ptr<spdlog::logger> LogShooter;
	};
}


#ifdef DEBUG

#define ENGINE_ERROR(...)		::Log::Logger::getLoggerEngine()->error(__VA_ARGS__)
#define ENGINE_INFO(...)		::Log::Logger::getLoggerEngine()->info(__VA_ARGS__)

#define SHOOTER_ERROR(...)		::Log::Logger::getLoggerShooter()->error(__VA_ARGS__)
#define SHOOTER_INFO(...)		::Log::Logger::getLoggerShooter()->info(__VA_ARGS__)

#else

#define ENGINE_ERROR(...)		::Log::Logger::getLoggerEngine()->error(__VA_ARGS__)
#define ENGINE_INFO(...)		::Log::Logger::getLoggerEngine()->info(__VA_ARGS__)

#define SHOOTER_ERROR(...)		::Log::Logger::getLoggerShooter()->error(__VA_ARGS__)
#define SHOOTER_INFO(...)		::Log::Logger::getLoggerShooter()->info(__VA_ARGS__)

#endif // DEBUG


