#include "databases.h"

const FileRepresentation::Shapes& 
FileRepresentation::GetShapes(const Box3D& area) const {
	return geom_storage_.get_all_shapes();
}

HeatmapStorage::HeatmapStorage(std::vector<float> x_steps,
													 		 std::vector<float> y_steps,
															 std::vector<float> temperature) 
	: x_steps_(x_steps), y_steps_(y_steps) {
	const size_t num_of_nodes  = x_steps_.size() * y_steps_.size();
	layers_count_ = temperature.size() / num_of_nodes;
	for (size_t i = 0; i < layers_count_;) {
		std::vector<float>::const_iterator first 
				= temperature.begin() + (num_of_nodes * i);
		std::vector<float>::const_iterator last 
				= temperature.begin() + (num_of_nodes * ++i);
		heatmaps_.emplace_back(std::vector<float>(first, last));
	}
}
