#include "heatmap_normalizer.h"

#include <algorithm>
#include <cassert>
#include <cmath>
#include <iterator>
#include <numeric>
#include <ranges>
#include <span>
#include <utility>

#include "common.h"
#include "heatmap.h"
#include "log.h"

bool HeatmapNormalizer::float_eq(const float a, const float b) {
	return std::abs(a - b) < eps;
}

HeatmapNormalizer::HeatmapNormalizer(const HeatmapStorage& heatmap_storage,
                                     const size_t max_resolution)
		: resolution_(FindOptimalResolution(
					heatmap_storage.MinStep(),
					std::max(heatmap_storage.x_size(), heatmap_storage.y_size()),
					max_resolution)),
			x_new_step_(heatmap_storage.x_size() / (resolution_ - 1)),
			y_new_step_(heatmap_storage.y_size() / (resolution_ - 1)),
			x_steps_(heatmap_storage.x_steps()),
			y_steps_(heatmap_storage.y_steps()),
			x_representation_size_(heatmap_storage.x_size()),
			y_representation_size_(heatmap_storage.y_size()),
			env_temp_(heatmap_storage.environment_temperature()) {
	// store coordinates of the temperatures
	static constexpr size_t size_adjustment = 2;
	x_old_coords_.reserve(x_steps_.size() + size_adjustment);
	y_old_coords_.reserve(y_steps_.size() + size_adjustment);
	x_old_coords_.push_back(0.0f);
	y_old_coords_.push_back(0.0f);

	std::inclusive_scan(x_steps_.begin(), x_steps_.end(),
	                    std::back_inserter(x_old_coords_));
	std::inclusive_scan(y_steps_.begin(), y_steps_.end(),
	                    std::back_inserter(y_old_coords_));

	x_old_coords_.push_back(heatmap_storage.x_size());
	y_old_coords_.push_back(heatmap_storage.y_size());

	// check for correctness
	// assert(float_eq(x_old_coords_.back(), x_representation_size_) &&
	//        "Error in x_steps");
	// assert(float_eq(y_old_coords_.back(), y_representation_size_) &&
	//        "Error in y_steps");
	assert(x_old_coords_.size() == x_steps_.size() + size_adjustment);
	assert(y_old_coords_.size() == y_steps_.size() + size_adjustment);
}

Heatmap HeatmapNormalizer::BilinearInterpolateSlow(const Heatmap& heatmap) {
	Floats result;
	result.reserve(resolution_ * resolution_);
	for (size_t i = 0; i < resolution_; ++i) {
		for (size_t j = 0; j < resolution_; ++j) {
			float x_coord = j * x_new_step_;
			float y_coord = i * y_new_step_;
			auto [x_bounds, y_bounds, upper_values, lower_values] =
					FindBilinearInterpolationCell(heatmap, x_old_coords_, y_old_coords_,
			                                  {x_coord, y_coord}, env_temp_);
			//         r1
			// |--------------|
			// |       |      |
			// |       |      |
			// |       |      |
			// |-----result---|      |--->
			// |       |      |      |   x
			// |       |      |      |
			// |--------------|      ∨ y
			//         r2
			auto r1 = Lerp(x_bounds, upper_values, x_coord);
			auto r2 = Lerp(x_bounds, lower_values, x_coord);
			result.push_back(Lerp(y_bounds, {r1, r2}, y_coord));
		}
	}
	return Heatmap(std::move(result), resolution_, resolution_);
}

HeatmapNormalizer::Cell HeatmapNormalizer::FindBilinearInterpolationCell(
		const Heatmap& heatmap, const Floats& x_coords, const Floats& y_coords,
		FloatPair search_coords, float env_temp) {
	namespace ranges = std::ranges;

	assert(search_coords.first >= x_coords.front());
	assert(search_coords.second >= y_coords.front());
	// upper_bound searches for an element, that is greater than given value. So
	// we always get [begin() + 1, end()) from it here, that is good as
	// right/lower side of cell.
	auto lower_coord_it = ranges::upper_bound(y_coords, search_coords.second);
	if (lower_coord_it == y_coords.end())
		lower_coord_it = y_coords.end() - 1;
	auto upper_coord_it = lower_coord_it - 1;

	auto right_coord_it = ranges::upper_bound(x_coords, search_coords.first);
	if (right_coord_it == x_coords.end())
		right_coord_it = x_coords.end() - 1;
	auto left_coord_it = right_coord_it - 1;

	// if `upper_coord_it == y_coords.begin()`, it is an edge, and should be
	// treated differently
	auto upper_row_idx    = std::distance(y_coords.begin() + 1, upper_coord_it);
	auto lower_row_idx    = std::distance(y_coords.begin() + 1, lower_coord_it);
	auto left_column_idx  = std::distance(x_coords.begin() + 1, left_coord_it);
	auto right_column_idx = std::distance(x_coords.begin() + 1, right_coord_it);

	assert(upper_row_idx == lower_row_idx - 1);
	assert(left_column_idx == right_column_idx - 1);

	// `heatmap.row(-1)` probably UB, but in this case we reassign results
	auto upper_row = heatmap.row(upper_row_idx);
	auto lower_row = heatmap.row(lower_row_idx);

	FloatPair upper_values = {upper_row[left_column_idx],
	                          upper_row[right_column_idx]};
	FloatPair lower_values = {lower_row[left_column_idx],
	                          lower_row[right_column_idx]};

	if (upper_coord_it == y_coords.begin())
		upper_values = {env_temp, env_temp};
	if (lower_coord_it == y_coords.end() - 1)
		lower_values = {env_temp, env_temp};
	if (left_coord_it == x_coords.begin())
		upper_values.first = env_temp, lower_values.first = env_temp;
	if (right_coord_it == x_coords.end() - 1)
		upper_values.second = env_temp, lower_values.second = env_temp;

	return {
			.x_bounds     = {*left_coord_it, *right_coord_it},
			.y_bounds     = {*upper_coord_it, *lower_coord_it},
			.upper_values = upper_values,
			.lower_values = lower_values,
	};
}

// Unifies heatmap net via bilinear interpolation
Heatmap HeatmapNormalizer::BilinearInterpolate(const Heatmap& heatmap) {
	// TODO: make documentation
	Floats result;
	result.reserve(resolution_ * resolution_);
	LOG_DEBUG("Resolution: {}", resolution_);
	auto result_iterator = std::back_inserter(result);

	auto LerpRowHelper = [this, &result_iterator](auto row) {
		result_iterator =
				InterpolateRow(row, x_steps_, x_new_step_, result_iterator);
	};

	// First row
	LerpRowHelper(heatmap.row(0));
	// assert(result.size() == resolution_);
	size_t y_old_idx         = 0;
	float y_old_top_coord    = 0.0f;
	float y_old_bottom_coord = y_steps_[0];
	// float y_new_coord = 0;
	// All non edge rows
	for (float y_new_coord = y_new_step_;
	     y_new_coord < y_representation_size_ - y_new_step_;
	     y_new_coord += y_new_step_) {
		// if iterated through current old node, update it
		if (y_new_coord > y_old_bottom_coord) {
			y_old_top_coord = y_old_bottom_coord;
			y_old_bottom_coord += y_steps_[y_old_idx++];
		}
		auto top_old_row    = heatmap.row(y_old_idx);
		auto bottom_old_row = heatmap.row(y_old_idx + 1);
		// if (std::abs(y_new_coord - y_old_bottom_coord) < eps) {
		// 	LOG_DEBUG("y_new_coord == y_old_bottom_coord | {} == {}", y_new_coord,
		// y_old_bottom_coord); 	LerpRowHelper(bottom_old_row); 	continue;
		// }
		LOG_TRACE("y_new_coord = {}, y_old_bottom_coord = {}, ratio = {}",
		          y_new_coord, y_old_bottom_coord,
		          y_new_coord / y_old_bottom_coord - 1.0f);
		assert(y_new_coord <= y_old_bottom_coord);
		// [Saiel]: I am storing all of the interpolated values, but I could store
		// only the needed two for `InterpolateRow` iteration. Implemented as it is
		// for readability. Looking forward to implementation of
		// `std::views::zip_transform` in gcc and msvc
		// (https://en.cppreference.com/w/cpp/ranges/zip_transform_view).
		std::vector<float> interpolated_row;
		interpolated_row.reserve(heatmap.x_resolution());

		std::ranges::transform(
				top_old_row, bottom_old_row, std::back_inserter(interpolated_row),
				[top_coord = y_old_top_coord, bottom_coord = y_old_bottom_coord,
		     y_new_coord](auto r1, auto r2) {
					return Lerp({top_coord, bottom_coord}, {r1, r2}, y_new_coord);
				});
		LerpRowHelper(interpolated_row);
	}
	// The last row
	LerpRowHelper(heatmap.row(heatmap.y_resolution() - 1));
	LOG_TRACE("Final heatmap size: {}", result.size());
	// assert(result.size() == resolution_ * resolution_ &&
	//        "Bug with resolution and/or indices");
	return Heatmap(std::move(result), resolution_, resolution_,
	               {heatmap.min_temp(), heatmap.max_temp()});
}

// Normalizes heatmap values to range [0; 1] without lossing min/max temperature
// information
void HeatmapNormalizer::Normalize(Heatmap& heatmap) {
	auto min_max_diff = heatmap.max_temp() - heatmap.min_temp();
	for (size_t i = 0; i < heatmap.y_resolution(); ++i) {
		auto row = heatmap.row(i);
		std::ranges::transform(row, row.begin(),
		                       [&heatmap, min_max_diff](auto temp) {
														 return (temp - heatmap.min_temp()) / min_max_diff;
													 });
	}
}

size_t HeatmapNormalizer::FindOptimalResolution(const float min_old_step,
                                                const float max_dimension_size,
                                                const size_t max_resolution) {
	assert(max_resolution > 0);
	const auto old_max_resolution =
			static_cast<size_t>(max_dimension_size / min_old_step) + 1;

	// Minimal meanfull resolution for real heatmaps.
	static constexpr size_t min_resolution = 512;
	// Max resolution should be passed from renderer api restriction, but may be
	// less than min_resolution for tests and should be using in this case.
	size_t heatmap_resolution = std::min(min_resolution, max_resolution);
	while (heatmap_resolution < old_max_resolution &&
	       heatmap_resolution < max_resolution) {
		heatmap_resolution *= 2;
	}

	assert(max_dimension_size / heatmap_resolution < min_old_step);

	return std::min(heatmap_resolution, max_resolution);
}

float HeatmapNormalizer::Lerp(const std::pair<float, float> bound_coords,
                              const std::pair<float, float> values,
                              const float interpolation_coord) {
	const float overall_distance = bound_coords.second - bound_coords.first;

	// assert(std::abs(overall_distance) > eps);
	if (std::abs(overall_distance) < eps)
		return values.first;
	assert(interpolation_coord > bound_coords.first ||
	       float_eq(interpolation_coord, bound_coords.first));

	const float factor =
			(interpolation_coord - bound_coords.first) / overall_distance;
	return std::lerp(values.first, values.second, factor);
}

size_t HeatmapNormalizer::CalculateBatchSize(const float right_coord,
                                             const float start_coord,
                                             const float step_size) {
	assert(std::abs(step_size) > eps);

	// step_size = 1
	//
	// 1           2           3
	// |-----------|-----------|
	// start_coord						 right_coord
	//
	// In this case function may return 2 or 3 due to float roundness.
	// So we check if we got result, which may include right_coord with
	// incrementing, we are incrementing it.
	//
	size_t res = 1 + static_cast<size_t>((right_coord - start_coord) / step_size);
	if (std::abs((res + 1) * step_size - right_coord) < eps)
		++res;
	return res;
}
