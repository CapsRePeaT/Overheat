#include "databases.h"

#include <cmath>
#include <iterator>
#include <numeric>
#include <ranges>

#include "common.h"
#include "heatmap.h"
#include "heatmap_normalizer.h"
#include "log.h"

const FileRepresentation::Shapes& FileRepresentation::GetShapes(
		const Box3D& /*area*/) const {
	return geom_storage_.shapes();
}

MetadataPack FileRepresentation::GetMetadata(const GlobalId id) const {
	MetadataPack result;
	switch (id.type()) {
		case InstanceType::Shape: {
			const auto& shape = geom_storage_[id.id()];
			break;
		}
		case InstanceType::Layer: {
			const auto& layer = layers_[id.id()];
			const auto& lower_heatmap = heatmaps_[layer.bottom_heatmap_id()];
			std::string bottom = std::to_string(lower_heatmap.min_temp()) + " " +
			                     std::to_string(lower_heatmap.max_temp());
			result.emplace_back("bottom temp range", bottom);
			const auto& upper_heatmap = heatmaps_[layer.top_heatmap_id()];
			std::string top = std::to_string(upper_heatmap.min_temp()) + " " +
			                  std::to_string(upper_heatmap.max_temp());
			result.emplace_back("top temp range", top);
			break;
		}
		case InstanceType::Representation: {

			break;
		}
		default:
			break;
	}
	result.emplace_back("representation", std::to_string(id.representation_id()));
	result.emplace_back("id", std::to_string(id.id()));
	//const auto data_from_storage = metadata_storage_.GetData(id);
	//result.emplace_back(data_from_storage.begin(), data_from_storage.end());
	return result;
}

void FileRepresentation::InitLayers() {
	// we assume that heatmaps not loaded
	if (heatmaps_.heatmaps().empty()) {
		for (size_t layer_id = 0; layer_id < layers_shapes_.size(); ++layer_id) {
			GlobalId new_id(InstanceType::Layer, layer_id, id().representation_id());
			layers_.emplace_back(new_id, layer_id, layer_id + 1);
		}
		return;
	}
	if (heatmaps_.heatmaps().size() == 1) {
		LayerId layer_id = 0;
		GlobalId new_id(InstanceType::Layer, layer_id, id().representation_id());
		layers_.emplace_back(new_id, layer_id, layer_id);
	}
	for (size_t layer_id = 0; layer_id < heatmaps_.heatmaps().size() - 1; ++layer_id) {
		GlobalId new_id(InstanceType::Layer, layer_id, id().representation_id());
		layers_.emplace_back(new_id, layer_id, layer_id + 1);
	}
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

InstanceList FileRepresentation::GetInstanceList() const {
	InstanceList result(id());
	result.name = RepresentationName();
	for (const auto& layer_id : GetAllLayerIds()) {
		InstanceList layer_inst(layer_id);
		layer_inst.name = GetName(layer_id);
		for (const auto& shape_id : GetAllShapeIdsOfLayer(layer_id)) {
			InstanceList shape_inst(shape_id);
			shape_inst.name = GetName(shape_id);
			layer_inst.dependants.push_back(shape_inst);
		}
		result.dependants.push_back(layer_inst);
	}
	return result;
}

HeatmapStorage::HeatmapStorage(std::vector<float> x_steps,
                               std::vector<float> y_steps,
                               const std::vector<float>& temperature,
                               const float environment_temperature,
                               Box3D representation_borders_mv)
		: env_temp_(environment_temperature),
			min_step_(std::min(*std::ranges::min_element(x_steps),
                         *std::ranges::min_element(y_steps))),
			representation_borders_(std::move(representation_borders_mv)) {
	// filling coords
	auto FillCoords = [this](std::vector<float>& result,
	                         const std::vector<float>& steps, const Axis axis) {
		result.push_back(0);
		std::inclusive_scan(steps.begin(), steps.end(), std::back_inserter(result));
		result.push_back(representation_borders_.coordinates()[axis].second);
	};
	FillCoords(x_coords_, x_steps, Axis::X);
	FillCoords(y_coords_, y_steps, Axis::Y);

	// initializing heatmaps with border temperatures as environment_temperature_
	const size_t x_steps_count = x_steps.size();
	const size_t y_steps_count  = y_steps.size();
	const size_t x_coords_num   = x_coords_.size();
	const size_t y_coords_num   = y_coords_.size();
	const size_t heatmap_resolution = x_steps_count * y_steps_count;
	assert(temperature.size() % heatmap_resolution == 0 && "Not interger layers_count");
	layers_count_ = temperature.size() / heatmap_resolution;
	auto GetHitmapWithBorders = [this, x_steps_count, y_steps_count,
	                             x_coords_num, y_coords_num, heatmap_resolution,
	                             &temperature](const int current_layer) {
		Floats result;
		result.reserve(x_coords_num * y_coords_num);

		const auto heatmaps_start = temperature.begin() + current_layer * heatmap_resolution;
		std::span<const float> raw_heatmap(heatmaps_start, heatmap_resolution);
		// fill the first line
		std::fill_n(std::back_inserter(result), x_coords_num, env_temp_);
		// Now this assert is ambiguous due to heatmap_resolution initialization,
		// but god takes care of the safe
		assert(raw_heatmap.size() % x_steps_count == 0 && "Not integer rows count");
		for (auto row_start = raw_heatmap.begin();
		     row_start != raw_heatmap.end();
		     row_start = row_start + x_steps_count) {
			auto row_end = row_start + x_steps_count;
			result.push_back(env_temp_);
			std::copy(row_start, row_end, std::back_inserter(result));
			result.push_back(env_temp_);
		}
		// fill the last line
		std::fill_n(std::back_inserter(result), x_coords_num, env_temp_);
		return result;
	};

	for (size_t i = 0; i < layers_count_; ++i)
		heatmaps_.emplace(heatmaps_.begin(), GetHitmapWithBorders(i),
		                  x_coords_.size(), y_coords_.size());
}

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
