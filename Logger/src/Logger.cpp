#include "Logger.h"

#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>

namespace Log
{
	std::shared_ptr<spdlog::logger> Logger::LogEngine;
	std::shared_ptr<spdlog::logger> Logger::LogShooter;

	void Logger::init()
	{
		std::vector<spdlog::sink_ptr> LogSinks;
		LogSinks.emplace_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
		LogSinks.emplace_back(std::make_shared<spdlog::sinks::basic_file_sink_mt>("Log.log", true));

		LogSinks[0]->set_pattern("%^[%T] %n: %v%$");
		LogSinks[1]->set_pattern("[%T] [%l] %n: %v");

		LogEngine = std::make_shared<spdlog::logger>("ENGINE", begin(LogSinks), end(LogSinks));
		spdlog::register_logger(LogEngine);
		LogEngine->set_level(spdlog::level::trace);
		LogEngine->flush_on(spdlog::level::trace);

		LogShooter = std::make_shared<spdlog::logger>("SHOOTER", begin(LogSinks), end(LogSinks));
		spdlog::register_logger(LogShooter);
		LogShooter->set_level(spdlog::level::trace);
		LogShooter->flush_on(spdlog::level::trace);
	}

}
