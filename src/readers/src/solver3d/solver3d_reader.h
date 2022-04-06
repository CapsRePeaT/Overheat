#pragma once

#include <filesystem>
#include <optional>

#include "../../../core/include/ireader.h"
#include "solver3d_T2D.h"
#include "solver3d_TRM.h"
namespace fs = std::filesystem;

namespace Readers::Solver3d {
Solver3d_TRM read_geometry(const std::string& content);
Solver3d_T2D read_heatmap(std::ifstream& trm_file);

// Reader implementation for files similar to described in doc/virtex.txt
class Solver3dReader : public IReader {
 public:
	// trm_file file with geometry
	// t2d file with heatmap
  Solver3dReader(fs::path trm_file_mv, fs::path t2d_file_mv)
			: trm_file_(std::move(trm_file_mv)), t2d_file_(std::move(t2d_file_mv)) {
		load();
	};

 protected:
	void load() override;

 private:
  Solver3d_TRM load_geometry();
  Solver3d_T2D load_heatmap();

	fs::path trm_file_;
	fs::path t2d_file_;
};
}  // namespace Readers::Solver3d
