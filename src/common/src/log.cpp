#include "log.h"

#include <spdlog/common.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

// TODO: config level from entrypoint
#ifdef NDEBUG
auto log_level = spdlog::level::info;
#else
auto log_level = spdlog::level::trace;
#endif // NDEBUG

void Log::Init() {
	std::vector<spdlog::sink_ptr> log_sinks;
	log_sinks.emplace_back(
			std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
	log_sinks.emplace_back(std::make_shared<spdlog::sinks::basic_file_sink_mt>(
			"Overheat.log", true));

	// https://github.com/gabime/spdlog/wiki/3.-Custom-formatting#pattern-flags
	log_sinks[0]->set_pattern("[%T.%e] [%^%l%$]: %v");
	log_sinks[1]->set_pattern("[%T.%e] [%l]: %v");

	logger_ = std::make_shared<spdlog::logger>("Overheat", log_sinks.begin(),
	                                           log_sinks.end());
	spdlog::register_logger(logger_);
	logger_->set_level(log_level);
	logger_->flush_on(log_level);

	// TODO: change pure spdlog usages to macros defined in log.h,
	// or delete macros (TBD)
	spdlog::set_default_logger(logger_);
	logger_->warn("Default logger was changed!");
}
