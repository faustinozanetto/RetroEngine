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