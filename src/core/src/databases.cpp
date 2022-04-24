#include "databases.h"

#include <cassert>
#include <utility>

const FileRepresentation::Shapes& FileRepresentation::GetShapes(
		const Box3D& /*area*/) const {
	return geom_storage_.shapes();
}

MetadataPack FileRepresentation::GetShapeMetadata(const ShapeId /*id*/) const {
	MetadataPack result;
	assert(false && "not implemented");
	return result;
}

GobalIds FileRepresentation::GetAllLayerIds() const { 
	GobalIds result; 
	for (const auto& layer : layers_) 
		result.push_back(layer.id());
	return result;
}

GobalIds FileRepresentation::GetAllShapeIdsOfLayer(GlobalId layer_id) const {
	assert(layer_id.type() == InstanceType::Layer);
	GobalIds result;
	for (const auto& shape : geom_storage_.shapes()) 
		if (layer_id.id() == shape->layer_id())
			result.push_back(shape->id());
	return result;
}

HeatmapStorage::HeatmapStorage(std::vector<float> x_steps_mv,
                               std::vector<float> y_steps_mv,
                               const std::vector<float>& temperature,
                               Box3D design_borders)
		: x_steps_(std::move(x_steps_mv)),
			y_steps_(std::move(y_steps_mv)), 
		  design_borders_(std::move(design_borders)) {
	const size_t heatmap_resolution = x_steps_.size() * y_steps_.size();
	
	layers_count_ = temperature.size() / heatmap_resolution;
	for (size_t i = 0; i < layers_count_;) {
		auto first = std::next(
				temperature.cbegin(),
				heatmap_resolution * i);  // NOLINT(bugprone-narrowing-conversions)
		auto last = std::next(
				temperature.cbegin(),
				heatmap_resolution * ++i);  // NOLINT(bugprone-narrowing-conversions)
		heatmaps_.emplace_back(std::vector<float>(first, last));
	}
}
