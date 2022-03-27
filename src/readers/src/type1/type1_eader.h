#pragma once

#include <filesystem>
#include <optional>

#include "../../../core/include/ireader.h"
#include "type1_T2D.h"
#include "type1_TRM.h"
namespace fs = std::filesystem;

namespace Readers::Type1 {
TRM read_geometry(const std::string& content);
type1_T2D read_heatmap(std::ifstream& trm_file);

// Reader implementation for files similar to described in doc/virtex.txt
class Type1Reader : public IReader {
 public:
	// trm_file file with geometry
	// t2d file with heatmap
  Type1Reader(fs::path trm_file, fs::path t2d_file)
			: trm_file_(std::move(trm_file)), t2d_file_(std::move(t2d_file)) {
		load();
	};

 protected:
	void load() override;

 private:
	TRM load_geometry();
	type1_T2D load_heatmap();

	fs::path trm_file_;
	fs::path t2d_file_;
};
}  // namespace Readers
