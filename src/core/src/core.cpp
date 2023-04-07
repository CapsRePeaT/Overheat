#include "core.h"

#include <iostream>

#include "../../fem_solver/src/fem_solver.hpp"
#include "../../readers/src/solver2d/solver2d_reader.h"
#include "../../readers/src/solver3d/solver3d_reader.h"
#include "databases.h"

Core& Core::instance() {
	static Core instance;
	return instance;
}

GlobalId Core::LoadRepresentationWithHeatmap(std::string trm_file_path_mv,
                                             std::string t2d_file_path_mv,
                                             const GeometryType type) {
	GlobalId rep_id;
	switch (type) {
		case GeometryType::D3: {
			Readers::Solver3d::Solver3dReader reader_3d(std::move(trm_file_path_mv),
			                                            std::move(t2d_file_path_mv));
			FileRepresentation rep(reader_3d.geometry(), reader_3d.heatmaps(),
														 reader_3d.layers_shapes(),
														 reader_3d.shapes_metadata(),
														 reader_3d.ambient_temperature());
			rep_id = rep.id();
			representations_.emplace(std::make_pair(rep_id.representation_id(), rep));
			break;
		}
		case GeometryType::D2: {
			Readers::Solver2d::Solver2dReader reader_2d(std::move(trm_file_path_mv),
			                                            std::move(t2d_file_path_mv));
			FileRepresentation rep(reader_2d.geometry(), reader_2d.heatmaps(),
														 reader_2d.layers_shapes(),
											 			 reader_2d.shapes_metadata(),
												 		 reader_2d.ambient_temperature());
			rep_id = rep.id();
			representations_.emplace(std::make_pair(rep_id.representation_id(), rep));
			break;
		}
		default:
			assert(false && "undefined geom type");
	}
	representations_.at(rep_id.representation_id()).InitLayers();
	return rep_id;

	// TODO: If we create new file representation here,
	// how can reader set its id to shapes?
}

GlobalId Core::LoadRepresentation(const std::string& trm_file_path_mv) {
	Readers::Solver3d::Solver3dReader reader_3d(trm_file_path_mv, "");
	//representations_.emplace_back(
	//		reader_3d.geometry(), reader_3d.heatmaps(), reader_3d.layers_shapes(),
	//		reader_3d.shapes_metadata(), reader_3d.ambient_temperature());
	FileRepresentation rep(reader_3d.geometry(), reader_3d.heatmaps(),
		reader_3d.layers_shapes(),
		reader_3d.shapes_metadata(),
		reader_3d.ambient_temperature());
	const GlobalId rep_id = rep.id();
	representations_.emplace(rep_id.representation_id(), rep);
	representations_.at(rep_id.representation_id()).InitLayers();
	return rep_id;
}

void Core::CalculateHeat(FileRepresentation& file_rep, SolverSetup setup) {
	FemSolver solver;
	solver.Solve(file_rep, setup);
}

FileRepresentation& Core::GetRepresentation(const GlobalId id) {
	const auto rep_id = id.representation_id();
	assert(representations_.find(rep_id) != representations_.end() && "Invalid Id");
	return representations_.at(rep_id);
}

void Core::DeleteAllRepresentations() {
	representations_.clear();
}

void Core::DeleteRepresentation(const GlobalId id) {
	representations_.erase(id.representation_id());
}

 MetadataPack Core::GetMetadata(const GlobalId id) const {
	return GetRepresentation(id.representation_id()).GetMetadata(id);
}

/// For filling side widgets
InstanceList Core::GetRepresentationData(const RepresentationId representation_id) const {
	return GetRepresentation(representation_id).GetInstanceList();
}

const FileRepresentation& Core::GetRepresentation(
	const RepresentationId id) const {
	// in the future there might be another connection with id and container
	return representations_.at(id);
}
