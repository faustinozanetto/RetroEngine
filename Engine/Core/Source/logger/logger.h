#pragma once

#include "core/base.h"

// This ignores all warnings raised inside External headers
#pragma warning(push, 0)
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>
#pragma warning(pop)

namespace retro
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

#define RETRO_CORE_TRACE(...) ::retro::logger::get_engine_logger()->trace(__VA_ARGS__)
#define RETRO_CORE_INFO(...) ::retro::logger::get_engine_logger()->info(__VA_ARGS__)
#define RETRO_CORE_DEBUG(...) ::retro::logger::get_engine_logger()->debug(__VA_ARGS__)
#define RETRO_CORE_WARN(...) ::retro::logger::get_engine_logger()->warn(__VA_ARGS__)
#define RETRO_CORE_ERROR(...) ::retro::logger::get_engine_logger()->error(__VA_ARGS__)
#define RETRO_CORE_CRITICAL(...) ::retro::logger::get_engine_logger()->critical(__VA_ARGS__)

#define RETRO_APP_TRACE(...) ::retro::logger::get_engine_logger()->trace(__VA_ARGS__)
#define RETRO_APP_INFO(...) ::retro::logger::get_engine_logger()->info(__VA_ARGS__)
#define RETRO_APP_DEBUG(...) ::retro::logger::get_engine_logger()->debug(__VA_ARGS__)
#define RETRO_APP_WARN(...) ::retro::logger::get_engine_logger()->warn(__VA_ARGS__)
#define RETRO_APP_ERROR(...) ::retro::logger::get_engine_logger()->error(__VA_ARGS__)
#define RETRO_APP_CRITICAL(...) ::retro::logger::get_engine_logger()->critical(__VA_ARGS__)
