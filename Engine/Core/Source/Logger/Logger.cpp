#include "pch.h"

#include "Logger.h"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_sinks.h"
#include "spdlog/sinks/base_sink.h"
#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/ostream_sink.h"

namespace Retro::Core
{
    std::shared_ptr<spdlog::logger> Logger::s_EngineLogger;

    void Logger::Initialize()
    {
        std::vector<spdlog::sink_ptr> sinks;
        sinks.push_back(std::make_shared<spdlog::sinks::stdout_sink_mt>());
        sinks.push_back(std::make_shared<spdlog::sinks::basic_file_sink_mt>("Logs/Engine.log", true));

        s_EngineLogger = std::make_shared<spdlog::logger>("MechaEngine", begin(sinks), end(sinks));
        register_logger(s_EngineLogger);
        // Customization
        s_EngineLogger->set_level(spdlog::level::trace);
        s_EngineLogger->set_pattern("[%n] [Time: %H:%M:%S] [Level: %l] %v");
        s_EngineLogger->flush_on(spdlog::level::trace);
    }

    void Logger::Line()
    {
        s_EngineLogger->info("");
        s_EngineLogger->info("=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*=*");
        s_EngineLogger->info("");
    }

    void Logger::Info(const std::string& message)
    {
        // Use spdlog to print message with formatting
        s_EngineLogger->info(message.c_str());
    }

    void Logger::Warning(const std::string& message)
    {
        s_EngineLogger->warn(message.c_str());
    }

    void Logger::Error(const std::string& message)
    {
        s_EngineLogger->error(message.c_str());
    }

    void Logger::Critical(const std::string& message)
    {
        s_EngineLogger->critical(message.c_str());
    }

    void Logger::Trace(const std::string& message)
    {
        s_EngineLogger->trace(message.c_str());
    }
}
