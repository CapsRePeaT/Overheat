#include "heatmap_normalizer.h"

#include <sys/ucontext.h>

#include <algorithm>
#include <cassert>
#include <cmath>
#include <iterator>
#include <numeric>
#include <ranges>
#include <span>

#include "common.h"
#include "heatmap.h"
#include "log.h"

bool HeatmapNormalizer::float_eq(const float a, const float b) {
	return std::abs(a - b) < eps;
}

HeatmapNormalizer::HeatmapNormalizer(const float min_step, const float x_size,
                                     const float y_size,
                                     const size_t max_resolution,
                                     const Floats& x_steps,
                                     const Floats& y_steps)
		: resolution_(FindOptimalResolution(min_step, std::max(x_size, y_size),
                                        max_resolution)),
			x_new_step_(x_size / (resolution_ - 1)),
			y_new_step_(y_size / (resolution_ - 1)),
			x_steps_(x_steps),
			y_steps_(y_steps),
			y_representation_size_(y_size) {}

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
Heatmap HeatmapNormalizer::Normalize(Heatmap heatmap_mv) {
	for (size_t i = 0; i < heatmap_mv.y_resolution(); ++i)
		for (auto& temperature : heatmap_mv.row(i)) {
			auto min_max_diff = heatmap_mv.max_temp() - heatmap_mv.min_temp();
			temperature       = (temperature - heatmap_mv.min_temp()) / min_max_diff;
		}
	return heatmap_mv;
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
	// less than minimal for test so it should be using in this case.
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

	assert(std::abs(overall_distance) > eps);
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
	auto r1    = right_coord - start_coord;
	auto r2    = r1 / step_size;
	auto r3    = static_cast<size_t>(r2);
	size_t res = 1 + static_cast<size_t>((right_coord - start_coord) / step_size);
	if (std::abs((res + 1) * step_size - right_coord) < eps)
		++res;
	return res;
}
