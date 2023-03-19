#include "core.h"

#include <iostream>

#include "../../fem_solver/src/fem_solver.hpp"
#include "../../readers/src/solver2d/solver2d_reader.h"
#include "../../readers/src/solver3d/solver3d_reader.h"
#include "databases.h"

GlobalId Core::LoadRepresentationWithHeatmap(std::string trm_file_path_mv,
                                             std::string t2d_file_path_mv,
                                             const GeometryType type) {
	switch (type) {
		case GeometryType::D3: {
			Readers::Solver3d::Solver3dReader reader_3d(std::move(trm_file_path_mv),
			                                            std::move(t2d_file_path_mv));
			representations_.emplace_back(reader_3d.geometry(), reader_3d.heatmaps(),
			                              reader_3d.layers_shapes(),
			                              reader_3d.shapes_metadata(),
			                              reader_3d.ambient_temperature());
			break;
		}
		case GeometryType::D2: {
			Readers::Solver2d::Solver2dReader reader_2d(std::move(trm_file_path_mv),
			                                            std::move(t2d_file_path_mv));
			representations_.emplace_back(reader_2d.geometry(), reader_2d.heatmaps(),
			                              reader_2d.layers_shapes(),
			                              reader_2d.shapes_metadata(),
			                              reader_2d.ambient_temperature());
			break;
		}
		default:
			assert(false && "undefined geom type");
	}
	representations_.back().InitLayers();
	return representations_.back().id();

	// TODO: If we create new file representation here,
	// how can reader set its id to shapes?
}

GlobalId Core::LoadRepresentation(const std::string& trm_file_path_mv) {
	// FIXME add real representation
	Readers::Solver3d::Solver3dReader reader_3d(trm_file_path_mv, "");
	representations_.emplace_back(
			reader_3d.geometry(), reader_3d.heatmaps(), reader_3d.layers_shapes(),
			reader_3d.shapes_metadata(), reader_3d.ambient_temperature());
	representations_.back().InitLayers();
	return representations_.back().id();
}

void Core::CalculateHeat(FileRepresentation& file_rep) {
	FemSolver solver;
	solver.Solve(file_rep);
}
