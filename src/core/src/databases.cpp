#include "databases.h"

#include <cassert>
#include <utility>

const FileRepresentation::Shapes& FileRepresentation::GetShapes(
		const Box3D& area) const {
	return geom_storage_.get_all_shapes();
}

MetadataPack FileRepresentation::GetShapeMetadata(Id id) const {
	MetadataPack result;
	assert(false && "not implemented");
	return result;
}

HeatmapStorage::HeatmapStorage(std::vector<float> x_steps,
                               std::vector<float> y_steps,
                               const std::vector<float>& temperature)
		: x_steps_(std::move(x_steps)), y_steps_(std::move(y_steps)) {
	const size_t num_of_nodes = x_steps_.size() * y_steps_.size();
	layers_count_             = temperature.size() / num_of_nodes;
	for (size_t i = 0; i < layers_count_;) {
		auto first = temperature.cbegin() + (num_of_nodes * i);
		auto last  = temperature.cbegin() + (num_of_nodes * ++i);
		heatmaps_.emplace_back(std::vector<float>(first, last));
	}
}
