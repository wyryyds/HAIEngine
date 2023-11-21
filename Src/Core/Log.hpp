#pragma once
#include "Core.hpp"

#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"

#include <memory>

#define LOG_Error(...)  ::HAIEngine::Log::GetCoreLogger()->error(__VA_ARGS__)
#define LOG_WARN(...)   ::HAIEngine::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define LOG_INFO(...)   ::HAIEngine::Log::GetCoreLogger()->info(__VA_ARGS__)
#define LOG_TRACE(...)  ::HAIEngine::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define LOG_FATAL(...)  ::HAIEngine::Log::GetCoreLogger()->fatal(__VA_ARGS__)

#define HE_CORE_ERROR(...)  ::HAIEngine::Log::GetClientLogger()->error(__VA_ARGS__)
#define HE_CORE_WARN(...)   ::HAIEngine::Log::GetClientLogger()->warn(__VA_ARGS__)
#define HE_CORE_INFO(...)   ::HAIEngine::Log::GetClientLogger()->info(__VA_ARGS__)
#define HE_CORE_TRACE(...)  ::HAIEngine::Log::GetClientLogger()->trace(__VA_ARGS__)
#define HE_CORE_FATAL(...)  ::HAIEngine::Log::GetClientLogger()->fatal(__VA_ARGS__)

namespace HAIEngine
{

	class Log
	{
	public:
		Log();
		~Log();

		static void Init();

		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_coreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_clientLogger; }

	private:
		static std::shared_ptr<spdlog::logger> s_coreLogger;
		static std::shared_ptr<spdlog::logger> s_clientLogger;
	};
}

