#include "databases.h"

#include <cassert>
#include <numeric>
#include <ranges>
#include <utility>

#include "heatmap.h"
#include "log.h"

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
	for (const auto& layer : layers_) result.push_back(layer.id());
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
                               Box3D representation_borders_mv)
		: x_steps_(std::move(x_steps_mv)),
			y_steps_(std::move(y_steps_mv)),
			representation_borders_(std::move(representation_borders_mv)) {
	const size_t heatmap_resolution = (x_steps_.size()) * (y_steps_.size());

	LOG_DEBUG("Design borders: {} {}",
	          representation_borders_.coordinates()[0].second,
	          representation_borders_.coordinates()[1].second);
	LOG_DEBUG("Sum of x_steps: {}",
	          std::accumulate(x_steps_.begin(), x_steps_.end(), 0.0f));
	LOG_DEBUG("Sum of y_steps: {}",
	          std::accumulate(y_steps_.begin(), y_steps_.end(), 0.0f));

	assert(temperature.size() % heatmap_resolution == 0 &&
	       "Not interger layers_count");
	layers_count_ = temperature.size() / heatmap_resolution;
	LOG_DEBUG("Got {} layers", layers_count_);
	for (size_t i = 0; i < layers_count_;) {
		auto first = std::next(
				temperature.cbegin(),
				heatmap_resolution * i);  // NOLINT(bugprone-narrowing-conversions)
		auto last = std::next(
				temperature.cbegin(),
				heatmap_resolution * ++i);  // NOLINT(bugprone-narrowing-conversions)
		heatmaps_.emplace(heatmaps_.begin(), std::vector<float>(first, last),
		                  x_steps_.size(), y_steps_.size());
	}
}

float HeatmapStorage::MinStep() const {
	return std::min(*std::ranges::min_element(x_steps_),
	                *std::ranges::min_element(y_steps_));
}
