#include "Log.h"
#include "spdlog/sinks/stdout_color_sinks.h"

std::shared_ptr<spdlog::logger> Log::s_LogicLogger;
std::shared_ptr<spdlog::logger> Log::s_CoreLogger;
std::shared_ptr<spdlog::logger> Log::s_RendererLogger;
std::shared_ptr<spdlog::logger> Log::s_NetworkLogger;

void Log::Init() {
	spdlog::set_pattern("%^[%T] %n: %v%$");

	s_LogicLogger = spdlog:: stdout_color_mt("Logic");
	s_LogicLogger->set_level(spdlog::level::trace);

	s_CoreLogger = spdlog::stdout_color_mt("Core");
	s_CoreLogger->set_level(spdlog::level::trace);

	s_RendererLogger = spdlog::stdout_color_mt("Renderer");
	s_RendererLogger->set_level(spdlog::level::trace);

	s_NetworkLogger = spdlog::stdout_color_mt("Network");
	s_NetworkLogger->set_level(spdlog::level::trace);
}
