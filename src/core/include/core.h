#pragma once

#include <cassert>
#include <string>

#include "common.h"
#include "databases.h"
#include "shapes.h"

using NameAndId = std::pair<std::string, GlobalId>;
using NameAndIds = std::vector<NameAndId>;

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
	[[nodiscard]] MetadataPack GetShapeMetadata(const GlobalId id) const {
		return GetRepresentation(id.representation_id()).GetShapeMetadata(id.id());
	}
	
	/// For filling side widgets
	NameAndIds GetRepresentationsData() const;
	NameAndIds GetLayersData(GlobalId representation_id) const;
	NameAndIds GetShapesData(GlobalId layer_id) const;

 private:
	Core() = default;
	[[nodiscard]] const FileRepresentation& GetRepresentation(
			const RepresentationId id) const {
		// in the future there might be another connection with id and container
		return representations_.at(id);
	}
	std::vector<FileRepresentation> representations_;
};
