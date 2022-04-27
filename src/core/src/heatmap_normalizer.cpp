#include "heatmap_normalizer.h"

#include <bits/ranges_algo.h>

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <iterator>
#include <ranges>
#include <span>

#include "common.h"
#include "heatmap.h"

namespace {

// For float comparisons
constexpr float eps = 0.0001f;

size_t FindOptimalResolution(float min_old_step, float max_dimension_size,
                             size_t max_resolution) {
	const auto old_max_resolution =
			static_cast<size_t>(max_dimension_size / min_old_step) + 1;

	size_t heatmap_resolution = 256;
	do {
		heatmap_resolution *= 2;
	} while (heatmap_resolution > old_max_resolution &&
	         heatmap_resolution < max_resolution);

	return std::min(heatmap_resolution, max_resolution);
}

float Lerp(std::pair<float, float> bound_coords, std::pair<float, float> values,
           const float left_distance) {
	const float overall_distance = bound_coords.second - bound_coords.first;
	const float factor = (left_distance - bound_coords.first) / overall_distance;
	return std::lerp(values.first, values.second, factor);
}

size_t CalculateBatchSize(float right_coord, float start_coord,
                          float step_size) {
	return static_cast<size_t>(1 + (right_coord - start_coord) / step_size);
}

/**
 * @brief Interpolates several values in current bounds by bounds coordinates,
 * starting coordinate and step size. Written with inspiration from STL.
 *
 * @tparam OutputIt - must meet the requirements of LegacyOutputIterator.
 * @param bound_coords - pair of the interpolation values coordinates. Requires
 * `bound_coords.first < bound_coords.second`
 * @param interpolating_values - pair of the actual interpolating values
 * @param start_coord - first coordinate of the interpolation result
 * @param step_size - distance between interpolation result
 * @param d_first - the beginning of the destination range
 * @return OutputIt - Output iterator to the element past the last interpolation
 * result
 */
template <typename OutputIt>
OutputIt BatchLerp(const std::pair<float, float> bound_coords,
                   const std::pair<float, float> values, float start_coord,
                   const float step_size, OutputIt d_first) {
	auto batch_size =
			CalculateBatchSize(bound_coords.second, start_coord, step_size);
	for (; batch_size > 0; --batch_size) {
		// do not interpolate if last value is the right bound
		if (std::abs(start_coord - bound_coords.second) < eps)
			return d_first;

		// by STL style
		*d_first++ = Lerp(bound_coords, values, start_coord);
		start_coord += step_size;
	}
	return d_first;
}

template <typename OutputIt>
OutputIt InterpolateRow(std::span<const float> row, const Floats& steps,
                        float step_size, OutputIt d_first) {
	float new_coord       = step_size;
	float old_right_coord = 0.0f;

	auto row_iter = row.begin();

	*d_first++ = *row_iter;
	for (auto step : steps) {
		float old_left_coord = old_right_coord;
		old_right_coord      = old_right_coord + step;

		const float r1 = *row_iter++;
		const float r2 = *row_iter;

		d_first = BatchLerp({old_left_coord, old_right_coord}, {r1, r2}, new_coord,
		                    step_size, d_first);
		new_coord += step_size * static_cast<float>(CalculateBatchSize(
																 old_right_coord, new_coord, step_size));
	}
	*d_first++ = *row_iter;
	return d_first;
}

}  // namespace

HeatmapNormalizer::HeatmapNormalizer(const float min_step, const float x_size,
                                     const float y_size,
                                     const size_t max_resolution,
                                     const Floats& x_steps,
                                     const Floats& y_steps)
		: resolution_(FindOptimalResolution(min_step, std::max(x_size, y_size),
                                        max_resolution)),
			x_new_step_(x_size / resolution_),
			y_new_step_(y_size / resolution_),
			x_steps_(x_steps),
			y_steps_(y_steps) {}

Floats HeatmapNormalizer::Normalize(const Heatmap& heatmap) {
	// TODO: make documentation
	Floats result(resolution_ * resolution_);
	auto result_iterator = result.begin();

	auto LerpRowHelper = [&steps = x_steps_, step_size = x_new_step_,
	                      &result_iterator](auto row) {
		result_iterator = InterpolateRow(row, steps, step_size, result_iterator);
	};

	LerpRowHelper(heatmap.row(0));

	// reset current y step idx and coordinate
	size_t y_old_idx         = 0;
	float y_old_top_coord    = 0.0f;
	float y_old_bottom_coord = y_steps_[0];

	for (size_t i : std::views::iota(1u, resolution_ - 1)) {
		// if iterated through current old node, update it
		float y_new_coord = y_new_step_ * i;
		if (y_new_coord > y_old_bottom_coord) {
			y_old_top_coord = y_old_bottom_coord;
			y_old_bottom_coord += y_steps_[y_old_idx];
		}
		auto top_old_row    = heatmap.row(y_old_idx++);
		auto bottom_old_row = heatmap.row(y_old_idx);
		// [Saiel]: We are storing all of the interpolated values, but we can store
		// only the needed two for `InterpolateRow` iteration. Implemented as it is
		// for readability. Waiting of `std::views::zip_transform`
		// (https://en.cppreference.com/w/cpp/ranges/zip_transform_view).
		std::vector<float> interpolated_row;
		interpolated_row.reserve(heatmap.x_resolution());

		std::ranges::transform(
				top_old_row, bottom_old_row, std::back_inserter(interpolated_row),
				[top_coord = y_old_top_coord, bottom_coord = y_old_bottom_coord,
		     top_distance = y_new_coord - y_old_top_coord](auto r1, auto r2) {
					return Lerp({top_coord, bottom_coord}, {r1, r2}, top_distance);
				});
		LerpRowHelper(interpolated_row);
	}
	LerpRowHelper(heatmap.row(heatmap.y_resolution() - 1));
	assert(result_iterator == result.end() &&
	       "Bug with resolution and/or indices");
	return result;
}
