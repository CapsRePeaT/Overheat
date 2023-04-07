#pragma once

#include <cassert>
#include <string>
#include <iostream>

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
	GlobalId LoadRepresentationWithHeatmap(std::string trm_file_path_mv,
	                                       std::string t2d_file_path_mv,
	                                       GeometryType type);
	GlobalId LoadRepresentation(const std::string& trm_file_path_mv);
	FileRepresentation& GetRepresentation(const GlobalId id) {
		const auto rep_id = id.representation_id();
		assert(representations_.size() > rep_id && "Invalid Id");
		return representations_[rep_id];
	}
	[[nodiscard]] MetadataPack GetMetadata(const GlobalId id) const {
		return GetRepresentation(id.representation_id()).GetMetadata(id);
	}
	/// For filling side widgets
	InstanceList GetRepresentationData(const size_t representation_id) const {
		return GetRepresentation(representation_id).GetInstanceList();
	}
	void CalculateHeat(FileRepresentation& file_rep, SolverSetup setup);

 private:
	Core() {};
	[[nodiscard]] const FileRepresentation& GetRepresentation(
			const RepresentationId id) const {
		// in the future there might be another connection with id and container
		return representations_.at(id);
	}
	std::vector<FileRepresentation> representations_;

};
