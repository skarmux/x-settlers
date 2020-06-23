#pragma once

#include "spdlog/spdlog.h"

class Log
{
public:
	static void Init();

	inline static std::shared_ptr<spdlog::logger>& GetLogicLogger() { return s_LogicLogger; }
	inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
	inline static std::shared_ptr<spdlog::logger>& GetRendererLogger() { return s_RendererLogger; }
	inline static std::shared_ptr<spdlog::logger>& GetNetworkLogger() { return s_NetworkLogger; }

private:
	static std::shared_ptr<spdlog::logger> s_LogicLogger;
	static std::shared_ptr<spdlog::logger> s_CoreLogger;
	static std::shared_ptr<spdlog::logger> s_RendererLogger;
	static std::shared_ptr<spdlog::logger> s_NetworkLogger;
};

// Logic log macros
#define LOGIC_TRACE(...) Log::GetLogicLogger()->trace(__VA_ARGS__)
#define LOGIC_INFO(...)  Log::GetLogicLogger()->info(__VA_ARGS__)
#define LOGIC_WARN(...)  Log::GetLogicLogger()->warn(__VA_ARGS__)
#define LOGIC_ERROR(...) Log::GetLogicLogger()->error(__VA_ARGS__)
#define LOGIC_FATAL(...) Log::GetLogicLogger()->fatal(__VA_ARGS__)

// Core log macros
#define CORE_TRACE(...) Log::GetCoreLogger()->trace(__VA_ARGS__)
#define CORE_INFO(...)  Log::GetCoreLogger()->info(__VA_ARGS__)
#define CORE_WARN(...)  Log::GetCoreLogger()->warn(__VA_ARGS__)
#define CORE_ERROR(...) Log::GetCoreLogger()->error(__VA_ARGS__)
#define CORE_FATAL(...) Log::GetCoreLogger()->fatal(__VA_ARGS__)

// Renderer log macros
#define RENDERER_TRACE(...) Log::GetRendererLogger()->trace(__VA_ARGS__)
#define RENDERER_INFO(...)  Log::GetRendererLogger()->info(__VA_ARGS__)
#define RENDERER_WARN(...)  Log::GetRendererLogger()->warn(__VA_ARGS__)
#define RENDERER_ERROR(...) Log::GetRendererLogger()->error(__VA_ARGS__)
#define RENDERER_FATAL(...) Log::GetRendererLogger()->fatal(__VA_ARGS__)

// Network log macros
#define NETWORK_TRACE(...) Log::GetNetworkLogger()->trace(__VA_ARGS__)
#define NETWORK_INFO(...)  Log::GetNetworkLogger()->info(__VA_ARGS__)
#define NETWORK_WARN(...)  Log::GetNetworkLogger()->warn(__VA_ARGS__)
#define NETWORK_ERROR(...) Log::GetNetworkLogger()->error(__VA_ARGS__)
#define NETWORK_FATAL(...) Log::GetNetworkLogger()->fatal(__VA_ARGS__)

#ifdef ENABLE_ASSERTS
	#define CORE_ASSERT(x,...) { if(!(x)) { CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak();) }}
#else
	#define CORE_ASSERT(x,...)
#endif
