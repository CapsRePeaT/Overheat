#include "core.h"

#include <iostream>

#include "../../readers/src/solver3d/solver3d_reader.h"

void Core::LoadGeometry(std::string trm_file_path_mv,
                        std::string t2d_file_path_mv) {
	// TODO make proper interface class usage
	Readers::Solver3d::Solver3dReader reader(std::move(trm_file_path_mv),
	                                         std::move(t2d_file_path_mv));
	FileRepresentation new_representation;
	// TODO: If we create new file representation here,
	// how can reader set its id to shapes?
	// TODO: BTW, we assign objects to getters here.
	//  1. We need setter for this operation
	//  2. Copy of 2 huge objects. Should be using pointers/references 
	//     or anything else to avoid it
	new_representation.geom_storage() = reader.geometry();
	new_representation.heatmaps()     = reader.heatmaps();
	representations_.emplace_back(std::move(new_representation));
}
