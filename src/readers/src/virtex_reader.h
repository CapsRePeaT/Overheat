#pragma once

#include <filesystem>
#include <optional>

#include "../../core/include/ireader.h"
#include "T2D.h"
#include "TRM.h"

namespace fs = std::filesystem;

namespace Readers {
TRM read_geometry(const std::string& content);
T2D read_heatmap(std::ifstream& trm_file);

// Reader implementation for files similar to described in doc/virtex.txt
class VirtexReader : public IReader {
 public:
	// trm_file file with geometry
	// t2d file with heatmap
	VirtexReader(fs::path trm_file, fs::path t2d_file)
			: trm_file_(std::move(trm_file)), t2d_file_(std::move(t2d_file)) {
		load();
	};

 protected:
	void load() override;

 private:
	TRM load_geometry();
	T2D load_heatmap();

	fs::path trm_file_;
	fs::path t2d_file_;
};
}  // namespace Readers
