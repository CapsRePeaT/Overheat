#pragma once

#include <spdlog/spdlog.h>

class Log {
 public:
	static void Init();
	[[nodiscard]] inline static std::shared_ptr<spdlog::logger> GetLogger() {
		return logger_;
	}

 private:
	static std::shared_ptr<spdlog::logger> logger_;
};

#define LOG_TRACE(...) Log::GetLogger()->trace(__VA_ARGS__)
#define LOG_INFO(...) Log::GetLogger()->info(__VA_ARGS__)
#define LOG_WARN(...) Log::GetLogger()->warn(__VA_ARGS__)
#define LOG_ERROR(...) Log::GetLogger()->error(__VA_ARGS__)
#define LOG_CRITICAL(...)                                             \
	if (true) {                                                         \
		Log::GetLogger()->critical("In file {}::{}", __FILE__, __LINE__); \
		Log::GetLogger()->critical(__VA_ARGS__);                          \
		std::abort();                                                     \
	}
