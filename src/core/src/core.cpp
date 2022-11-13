#include "core.h"

#include <iostream>

#include "../../readers/src/solver3d/solver3d_reader.h"
#include "../../readers/src/solver2d/solver2d_reader.h"
#include "databases.h"

GlobalId Core::LoadRepresentation(std::string trm_file_path_mv,
                        std::string t2d_file_path_mv, 
	                      const GeometryType type) {
	switch (type) {
		case GeometryType::D3: {
		  Readers::Solver3d::Solver3dReader reader_3d(std::move(trm_file_path_mv),
	                                                std::move(t2d_file_path_mv));
			representations_.emplace_back(reader_3d.geometry(), reader_3d.heatmaps());
			break;
		}
		case GeometryType::D2: {
			Readers::Solver2d::Solver2dReader reader_2d(std::move(trm_file_path_mv),
	                                                std::move(t2d_file_path_mv));
			representations_.emplace_back(reader_2d.geometry(), reader_2d.heatmaps());
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
