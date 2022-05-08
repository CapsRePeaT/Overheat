#include "databases.h"

#include <cmath>
#include <numeric>
#include <ranges>

#include "heatmap.h"
#include "log.h"
#include "heatmap_normalizer.h"

const FileRepresentation::Shapes& FileRepresentation::GetShapes(
		const Box3D& /*area*/) const {
	return geom_storage_.shapes();
}

MetadataPack FileRepresentation::GetShapeMetadata(const ShapeId /*id*/) const {
	MetadataPack result;
	assert(false && "not implemented");
	return result;
}

GlobalIds FileRepresentation::GetAllLayerIds() const {
	GlobalIds result;
	for (const auto& layer : layers_) result.push_back(layer.id());
	return result;
}

GlobalIds FileRepresentation::GetAllShapeIdsOfLayer(GlobalId layer_id) const {
	assert(layer_id.type() == InstanceType::Layer);
	GlobalIds result;
	for (const auto& shape : geom_storage_.shapes())
		if (layer_id.id() == shape->layer_id())
			result.push_back(shape->id());
	return result;
}

HeatmapStorage::HeatmapStorage(std::vector<float> x_steps,
                               std::vector<float> y_steps,
                               const std::vector<float>& temperature,
															 const float environment_temperature,
                               Box3D representation_borders_mv)
		: environment_temperature_(environment_temperature),
			representation_borders_(std::move(representation_borders_mv)) {
	min_step_ = std::min(*std::ranges::min_element(x_steps),
	                     *std::ranges::min_element(y_steps));
	// filling coords
	auto FillCoords = [this]
	                  (std::vector<float>& result,
									   const std::vector<float>& steps, 
		                 const Axis axis) {
		result.push_back(0);
		for (const auto& step : steps) result.push_back(result.back() + step);
		result.push_back(representation_borders_.coordinates()[Axis::X].second);
	};
	FillCoords(x_coords_, x_steps, Axis::X);
	FillCoords(y_coords_, y_steps, Axis::Y);
	const size_t heatmap_resolution = (x_steps.size()) * (y_steps.size());
	assert(temperature.size() % heatmap_resolution == 0 && "Not interger layers_count");
	layers_count_ = temperature.size() / heatmap_resolution;

	auto GetHitmapWithBorders = [this, &heatmap_resolution, &temperature,
	                             &x_steps, &y_steps](const size_t layer_count) {
		Floats result;
		result.reserve(x_coords_.size() * y_coords_.size());
		// fill first line
		result.insert(result.end(), x_coords_.size(), environment_temperature_);
		for (size_t i = 0; i < y_steps.size(); ++i) {
			const size_t heatmap_start_index =
					layer_count * heatmap_resolution + x_steps.size() * i;
			result.push_back(environment_temperature_);
			auto first = std::next(temperature.cbegin(), heatmap_start_index);
			auto last =
					std::next(temperature.cbegin(), heatmap_start_index + x_steps.size());
			result.insert(result.end(), first, last);
			result.push_back(environment_temperature_);
		}
		result.insert(result.end(), x_coords_.size(), environment_temperature_);
		return result;
	};

	for (size_t i = 0; i < layers_count_; ++i)
		heatmaps_.emplace(heatmaps_.begin(),
			                GetHitmapWithBorders(i), 
			                x_coords_.size(),
		                  y_coords_.size());
	// HeatmapNormalizer normalizer(*this, 16000);
	// Heatmap test = normalizer.Normalize(heatmaps_[6]);
	// test.DebugPrint();
}

//float HeatmapStorage::MinStep() const {
//	auto MinRange = [](const Floats& floats) {
//		assert(floats.size() > 2);
//		float result = floats[1] - floats[0];
//		for (size_t i = 0; i < floats.size() - 1; ++i) {
//			const float iteration_result = floats[i + 1] - floats[i];
//			if (iteration_result < result)
//				result = iteration_result;
//		}
//		return result;
//	};
//	return std::min(MinRange(x_coords_), MinRange(y_coords_));
//}

float HeatmapStorage::max_temp() const {
	if (std::isnan(max_temp_cache_))
		SetMinMaxTempCaches();
	return max_temp_cache_;
}

float HeatmapStorage::min_temp() const {
	if (std::isnan(min_temp_cache_))
		SetMinMaxTempCaches();
	return min_temp_cache_;
}

void HeatmapStorage::SetMinMaxTempCaches() const {
	for (const auto& heatmap : heatmaps_) {
		min_temp_cache_ = std::min(heatmap.min_temp(), min_temp_cache_);
		max_temp_cache_ = std::max(heatmap.max_temp(), max_temp_cache_);
	}
}
