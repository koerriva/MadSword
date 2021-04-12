#pragma once

#include <memory>
#include "Core.h"
#include "spdlog/spdlog.h"

namespace MadSword {
	class MS_API Log {
	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;
		static std::shared_ptr<spdlog::logger> s_ClientLogger;
	public:
		static void Init();
		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }
	};
}

#define MS_CORE_TRACE(...)		::MadSword::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define MS_CORE_INFO(...)		::MadSword::Log::GetCoreLogger()->info(__VA_ARGS__)
#define MS_CORE_WARN(...)		::MadSword::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define MS_CORE_ERROR(...)		::MadSword::Log::GetCoreLogger()->error(__VA_ARGS__)
#define MS_CORE_CRITICAL(...)	::MadSword::Log::GetCoreLogger()->critical(__VA_ARGS__)

#define MS_TRACE(...)		::MadSword::Log::GetClientLogger()->trace(__VA_ARGS__)
#define MS_INFO(...)		::MadSword::Log::GetClientLogger()->info(__VA_ARGS__)
#define MS_WARN(...)		::MadSword::Log::GetClientLogger()->warn(__VA_ARGS__)
#define MS_ERROR(...)		::MadSword::Log::GetClientLogger()->error(__VA_ARGS__)
#define MS_CRITICAL(...)	::MadSword::Log::GetClientLogger()->critical(__VA_ARGS__)