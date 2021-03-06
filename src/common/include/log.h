#pragma once

#include <spdlog/spdlog.h>

class Log {
 public:
	static void Init();
	[[nodiscard]] inline static std::shared_ptr<spdlog::logger> GetLogger() {
		return logger_;
	}

 private:
	static inline std::shared_ptr<spdlog::logger> logger_;
};

//
// Basic usage:
// 1. LOG_DEBUG("The answer is {}.", 42)
//   prints "The answer is 42."
// 2. LOG_DEBUG("I'd rather be {1} than {0}.", "right", "happy")
//   prints "I'd rather be happy than right."
// More:
//   https://github.com/fmtlib/fmt#examples
//   https://github.com/gabime/spdlog#usage-samples
//
#define LOG_DEBUG(...) Log::GetLogger()->debug(__VA_ARGS__)
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

template <typename T>
struct fmt::formatter<std::vector<T>> {
	static constexpr auto parse(format_parse_context& ctx)
			-> decltype(ctx.begin()) {
		return ctx.end();
	}
	template <typename FormatContext>
	auto format(const std::vector<T>& v, FormatContext& ctx)
			-> decltype(ctx.out()) {
		auto out = format_to(ctx.out(), "vec{{");
		for (auto it = v.cbegin(); it < v.cend(); ++it) {
			out = format_to(out, "{}", *it);
			if (it != v.cend() - 1)
				out = format_to(out, ", ");
		}
		return format_to(out, "}}");
	}
};
