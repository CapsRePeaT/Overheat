#pragma once

#include <string>
#include <cassert>

#include "common.h"
#include "databases.h"
#include "shapes.h"

class Core {
 public:
	using Shapes = FileRepresentation::Shapes;

	static Core& instance() {
		static Core instance;
		return instance;
	}
	void LoadHeatmap(const std::string& file_name) {}
	void LoadGeometry(const std::string& trm_file, 
									  const std::string& t2d_file);
	// for now we have only one file loaded
	FileRepresentation& GetFirstFile() {
		assert(representations_.size() && "any representations available");
		return representations_[0];
	}

 private:
	Core() = default;
	std::vector<FileRepresentation> representations_;
};
