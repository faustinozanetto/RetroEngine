#pragma once

#include "Core/Base.h"

// This ignores all warnings raised inside External headers
#pragma warning(push, 0)
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>
#pragma warning(pop)


namespace Retro
{
	class Logger
	{
	public:
		static void Initialize();
		static std::shared_ptr<spdlog::logger> GetEngineLogger() { return s_EngineLogger; }
		static void Line();
		static void Info(const std::string& message);
		static void Warning(const std::string& message);
		static void Error(const std::string& message);
		static void Critical(const std::string& message);
		static void Trace(const std::string& message);

	private:
		static std::shared_ptr<spdlog::logger> s_EngineLogger;
	};
}

#define RETRO_CORE_TRACE(...)		::Retro::Logger::GetEngineLogger()->trace(__VA_ARGS__)
#define RETRO_CORE_INFO(...)		::Retro::Logger::GetEngineLogger()->info(__VA_ARGS__)
#define RETRO_CORE_DEBUG(...)		::Retro::Logger::GetEngineLogger()->debug(__VA_ARGS__)
#define RETRO_CORE_WARN(...)		::Retro::Logger::GetEngineLogger()->warn(__VA_ARGS__)
#define RETRO_CORE_ERROR(...)		::Retro::Logger::GetEngineLogger()->error(__VA_ARGS__)
#define RETRO_CORE_CRITICAL(...)	::Retro::Logger::GetEngineLogger()->critical(__VA_ARGS__)

#define RETRO_APP_TRACE(...)		::Retro::Logger::GetEngineLogger()->trace(__VA_ARGS__)
#define RETRO_APP_INFO(...)		::Retro::Logger::GetEngineLogger()->info(__VA_ARGS__)
#define RETRO_APP_DEBUG(...)		::Retro::Logger::GetEngineLogger()->debug(__VA_ARGS__)
#define RETRO_APP_WARN(...)		::Retro::Logger::GetEngineLogger()->warn(__VA_ARGS__)
#define RETRO_APP_ERROR(...)		::Retro::Logger::GetEngineLogger()->error(__VA_ARGS__)
#define RETRO_APP_CRITICAL(...)	::Retro::Logger::GetEngineLogger()->critical(__VA_ARGS__)
