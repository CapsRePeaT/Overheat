#pragma once

#include <filesystem>

#include "../../../core/include/ireader.h"
#include "type2_TRM.h"

namespace fs = std::filesystem;

namespace Readers::Type2 {
type2_TRM read_geometry(std::string& content);
// TODO implement read_heatmap(std::ifstream& trm_file);

// Reader implementation for files similar to described in doc/en9.txt.
class Type2Reader : public IReader {
 public:
	// trm_file file with geometry
	// t2d file with heatmap
  Type2Reader(fs::path trm_file, fs::path t2d_file)
			: trm_file_(std::move(trm_file)), t2d_file_(std::move(t2d_file)) {
		load();
	};

 protected:
	void load() override;

 private:
	type2_TRM load_geometry();

  fs::path trm_file_;
  fs::path t2d_file_;

};
}  // namespace Readers
