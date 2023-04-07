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

	static Core& instance();
	GlobalId LoadRepresentationWithHeatmap(std::string trm_file_path_mv,
	                                       std::string t2d_file_path_mv,
	                                       GeometryType type);
	GlobalId LoadRepresentation(const std::string& trm_file_path_mv);
	void CalculateHeat(FileRepresentation& file_rep, SolverSetup setup);
	FileRepresentation& GetRepresentation(const GlobalId id);
	void DeleteAllRepresentations();
	void DeleteRepresentation(const GlobalId id);
	[[nodiscard]] MetadataPack GetMetadata(const GlobalId id) const;
	/// For filling side widgets
	InstanceList GetRepresentationData(const RepresentationId representation_id) const;
 private:
	Core() {};
	[[nodiscard]] const FileRepresentation& GetRepresentation(const RepresentationId id) const;
	std::map<RepresentationId, FileRepresentation> representations_;
};
