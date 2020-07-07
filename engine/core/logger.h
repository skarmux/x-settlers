#pragma once

#include "spdlog/spdlog.h"

class Logger
{
public:
	static void init();

	inline static std::shared_ptr<spdlog::logger>& GetLogicLogger() { return s_LogicLogger; }
	inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
	inline static std::shared_ptr<spdlog::logger>& GetRendererLogger() { return s_RendererLogger; }
	inline static std::shared_ptr<spdlog::logger>& GetNetworkLogger() { return s_NetworkLogger; }
	inline static std::shared_ptr<spdlog::logger>& GetEventLogger() { return s_EventLogger; }

private:
	static std::shared_ptr<spdlog::logger> s_LogicLogger;
	static std::shared_ptr<spdlog::logger> s_CoreLogger;
	static std::shared_ptr<spdlog::logger> s_RendererLogger;
	static std::shared_ptr<spdlog::logger> s_NetworkLogger;
	static std::shared_ptr<spdlog::logger> s_EventLogger;
};

// Implementing logging macros for individual engine components

// Logic log macros
#define LOGIC_TRACE(...) Logger::GetLogicLogger()->trace(__VA_ARGS__)
#define LOGIC_INFO(...)  Logger::GetLogicLogger()->info(__VA_ARGS__)
#define LOGIC_WARN(...)  Logger::GetLogicLogger()->warn(__VA_ARGS__)
#define LOGIC_ERROR(...) Logger::GetLogicLogger()->error(__VA_ARGS__)
#define LOGIC_FATAL(...) Logger::GetLogicLogger()->fatal(__VA_ARGS__)

// Core log macros
#define CORE_TRACE(...) Logger::GetCoreLogger()->trace(__VA_ARGS__)
#define CORE_INFO(...)  Logger::GetCoreLogger()->info(__VA_ARGS__)
#define CORE_WARN(...)  Logger::GetCoreLogger()->warn(__VA_ARGS__)
#define CORE_ERROR(...) Logger::GetCoreLogger()->error(__VA_ARGS__)
#define CORE_FATAL(...) Logger::GetCoreLogger()->fatal(__VA_ARGS__)

// Core log macros
#define EVENT_TRACE(...) Logger::GetEventLogger()->trace(__VA_ARGS__)
#define EVENT_INFO(...)  Logger::GetEventLogger()->info(__VA_ARGS__)
#define EVENT_WARN(...)  Logger::GetEventLogger()->warn(__VA_ARGS__)
#define EVENT_ERROR(...) Logger::GetEventLogger()->error(__VA_ARGS__)
#define EVENT_FATAL(...) Logger::GetEventLogger()->fatal(__VA_ARGS__)

// Renderer log macros
#define RENDERER_TRACE(...) Logger::GetRendererLogger()->trace(__VA_ARGS__)
#define RENDERER_INFO(...)  Logger::GetRendererLogger()->info(__VA_ARGS__)
#define RENDERER_WARN(...)  Logger::GetRendererLogger()->warn(__VA_ARGS__)
#define RENDERER_ERROR(...) Logger::GetRendererLogger()->error(__VA_ARGS__)
#define RENDERER_FATAL(...) Logger::GetRendererLogger()->fatal(__VA_ARGS__)

// Network log macros
#define NETWORK_TRACE(...) Logger::GetNetworkLogger()->trace(__VA_ARGS__)
#define NETWORK_INFO(...)  Logger::GetNetworkLogger()->info(__VA_ARGS__)
#define NETWORK_WARN(...)  Logger::GetNetworkLogger()->warn(__VA_ARGS__)
#define NETWORK_ERROR(...) Logger::GetNetworkLogger()->error(__VA_ARGS__)
#define NETWORK_FATAL(...) Logger::GetNetworkLogger()->fatal(__VA_ARGS__)

#ifdef _MSC_VER
	#define DEBUG_BREAK() __debugbreak()
#else
	#define DEBUG_BREAK()
#endif

#define CORE_ASSERT(x,...) { if(!(x)) { CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); DEBUG_BREAK(); } }

