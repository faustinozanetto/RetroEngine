#include "pch.h"

#include "logger.h"

#include "spdlog/sinks/stdout_sinks.h"
#include "spdlog/sinks/base_sink.h"
#include "spdlog/sinks/ostream_sink.h"

namespace Retro
{
	std::shared_ptr<spdlog::logger> logger::s_engine_logger;

	void logger::initialize()
	{
		std::vector<spdlog::sink_ptr> sinks;
		sinks.push_back(std::make_shared<spdlog::sinks::stdout_sink_mt>());
		//  sinks.push_back(std::make_shared<spdlog::sinks::basic_file_sink_mt>("Logs/Engine.log", true));

		s_engine_logger = std::make_shared<spdlog::logger>("Retro Engine", begin(sinks), end(sinks));
		register_logger(s_engine_logger);
		// Customization
		s_engine_logger->set_level(spdlog::level::trace);
		s_engine_logger->set_pattern("[%n] [Time: %H:%M:%S] [Level: %l] %v");
		s_engine_logger->flush_on(spdlog::level::trace);
	}

	void logger::line()
	{
		s_engine_logger->info("");
		s_engine_logger->info("=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*");
		s_engine_logger->info("");
	}

	void logger::info(const std::string &message)
	{
		// Use spdlog to print message with formatting
		s_engine_logger->info(message.c_str());
	}

	void logger::warning(const std::string &message)
	{
		s_engine_logger->warn(message.c_str());
	}

	void logger::error(const std::string &message)
	{
		s_engine_logger->error(message.c_str());
	}

	void logger::critical(const std::string &message)
	{
		s_engine_logger->critical(message.c_str());
	}

	void logger::trace(const std::string &message)
	{
		s_engine_logger->trace(message.c_str());
	}
}
