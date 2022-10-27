#pragma once

#include "core/base.h"

// This ignores all warnings raised inside External headers
#pragma warning(push, 0)
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>
#pragma warning(pop)

namespace Retro
{
	class logger
	{
	public:
		static void initialize();
		static std::shared_ptr<spdlog::logger> get_engine_logger() { return s_engine_logger; }
		static void line();
		static void info(const std::string &message);
		static void warning(const std::string &message);
		static void error(const std::string &message);
		static void critical(const std::string &message);
		static void trace(const std::string &message);

	private:
		static std::shared_ptr<spdlog::logger> s_engine_logger;
	};
}

#define RETRO_CORE_TRACE(...) ::Retro::Logger::GetEngineLogger()->trace(__VA_ARGS__)
#define RETRO_CORE_INFO(...) ::Retro::Logger::GetEngineLogger()->info(__VA_ARGS__)
#define RETRO_CORE_DEBUG(...) ::Retro::Logger::GetEngineLogger()->debug(__VA_ARGS__)
#define RETRO_CORE_WARN(...) ::Retro::Logger::GetEngineLogger()->warn(__VA_ARGS__)
#define RETRO_CORE_ERROR(...) ::Retro::Logger::GetEngineLogger()->error(__VA_ARGS__)
#define RETRO_CORE_CRITICAL(...) ::Retro::Logger::GetEngineLogger()->critical(__VA_ARGS__)

#define RETRO_APP_TRACE(...) ::Retro::Logger::GetEngineLogger()->trace(__VA_ARGS__)
#define RETRO_APP_INFO(...) ::Retro::Logger::GetEngineLogger()->info(__VA_ARGS__)
#define RETRO_APP_DEBUG(...) ::Retro::Logger::GetEngineLogger()->debug(__VA_ARGS__)
#define RETRO_APP_WARN(...) ::Retro::Logger::GetEngineLogger()->warn(__VA_ARGS__)
#define RETRO_APP_ERROR(...) ::Retro::Logger::GetEngineLogger()->error(__VA_ARGS__)
#define RETRO_APP_CRITICAL(...) ::Retro::Logger::GetEngineLogger()->critical(__VA_ARGS__)
