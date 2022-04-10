#pragma once

#include <cassert>
#include <string>

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
	void LoadHeatmap(std::string file_path_mv) {}
	void LoadGeometry(std::string trm_file_path_mv, 
									  std::string t2d_file_path_mv,
	                  GeometryType type);
	// for now we have only one file loaded
	FileRepresentation& GetFirstFile() {
		assert(!representations_.empty() && "any representations available");
		return representations_[0];
	}
	[[nodiscard]] MetadataPack GetShapeMetadata(const GlobalShapeId id) const {
		return GetRepresentation(id.representation_id()).GetShapeMetadata(id.id());
	}

 private:
	Core() = default;
	[[nodiscard]] const FileRepresentation& GetRepresentation(
			const RepresentationId id) const {
		// in the future there might be another connection with id and container
		return representations_.at(id);
	}
	std::vector<FileRepresentation> representations_;
};
