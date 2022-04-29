#pragma once

#include <filesystem>
#include <memory>
#include <utility>


// TODO: add config file 
class Config {
 public:
	using Path = std::filesystem::path;
	static inline void Init(Path executable_path) {
		instance_ = std::unique_ptr<Config>(new Config(std::move(executable_path)));
	}
	// [Saiel] Because we need to initialize this in runtime and we do not want to
	// have an argument in `instance` for initialization, I implemented it with
	// unique_ptr
	static inline const Config& instance() { return *instance_; }

	[[nodiscard]] inline Path executable_path() const { return executable_path_; }
	[[nodiscard]] inline Path executable_dir_path() const {
		return executable_dir_path_;
	}

 private:
	explicit Config(Path executable_path)
			: executable_path_(std::move(executable_path)),
				executable_dir_path_(executable_path.parent_path()){};

	inline static std::unique_ptr<Config> instance_ = nullptr;

	Path executable_path_;
	Path executable_dir_path_;
};
