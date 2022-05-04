#pragma once

#include <iterator>

#include "common.h"
#include "databases.h"
#include "heatmap.h"
#include "log.h"

class HeatmapNormalizer {
 public:
	HeatmapNormalizer(float min_step, float x_size, float y_size,
	                  size_t max_resolution, const Floats& x_steps,
	                  const Floats& y_steps);
	Heatmap BilinearInterpolate(const Heatmap& heatmap);
	Heatmap Normalize(Heatmap heatmap_mv);

	/**
	 * @brief Finds minimal resolution, that would be a power of 2 and all cells
	 * would be less than min_old_step
	 *
	 * @param min_old_step - minimal node step in the heatmap
	 * @param max_dimension_size - maximal resolution of the either dimesion of
	 * the heatmap
	 * @param max_resolution - limit for the final resolution
	 * @return size_t
	 */
	[[nodiscard]] size_t FindOptimalResolution(float min_old_step,
	                                           float max_dimension_size,
	                                           size_t max_resolution);

	/**
	 * @brief Linear interpolation by coordinates
	 *
	 * @param bound_coords - value coordinates for interpolation
	 * @param values - values itself for interpolation
	 * @param interpolation_coord - coordinate, where compute interpolation result
	 * @return float - interpolation result
	 */
	[[nodiscard]] static float Lerp(std::pair<float, float> bound_coords,
	                                std::pair<float, float> values,
	                                float interpolation_coord);

	/**
	 * @brief Computes amount of possible interpolation result from `start_coord`
	 * to `right_coord` with given `step_size` including the first coordinate
	 *
	 * @param right_coord - interpolation right bound
	 * @param start_coord - the first coordinate of the interpolation results
	 * @param step_size - distance between interpolation results
	 * @return size_t - \f$\frac{right_coord - start_coord}{step_size} + 1 \f$
	 */
	[[nodiscard]] static size_t CalculateBatchSize(float right_coord,
	                                               float start_coord,
	                                               float step_size);

	/**
	 * @brief Interpolates several values in current bounds by bounds coordinates,
	 * starting coordinate and step size. Written with inspiration from STL.
	 *
	 * @tparam OutputIt - must meet the requirements of LegacyOutputIterator.
	 * @param bound_coords - pair of the interpolation values coordinates.
	 * Requires `bound_coords.first < bound_coords.second`
	 * @param interpolating_values - pair of the actual interpolating values
	 * @param start_coord - first coordinate of the interpolation result
	 * @param step_size - distance between interpolation result
	 * @param d_first - the beginning of the destination range
	 * @return OutputIt - Output iterator to the element past the last
	 * interpolation result
	 */
	template <typename OutputIt>
	static OutputIt BatchLerp(std::pair<float, float> bound_coords,
	                          std::pair<float, float> values, float start_coord,
	                          float step_size, OutputIt d_first);

	/**
	 * @brief Interpolates row with raw values, with each `row[i]` and `row[i+1]`
	 * values separated by `steps[i]`.
	 *
	 * @details Interpolating performs with BatchLerp for each pair of values.
	 *
	 * @tparam OutputIt - must meet the requirements of LegacyOutputIterator.
	 * @param row - values for interpolation
	 * @param steps - distances between row values
	 * @param step_size - distance between interpolation results
	 * @param d_first - the beginning of the destination range
	 * @return OutputIt - Output iterator to the element past the last
	 * interpolation result
	 */
	template <typename OutputIt>
	static OutputIt InterpolateRow(std::span<const float> row,
	                               const Floats& steps, float step_size,
	                               OutputIt d_first);

 private:
	// For float comparisons
	static constexpr float eps = 0.002f;
	static bool float_eq(float a, float b);

	const size_t resolution_;
	const float x_new_step_;
	const float y_new_step_;
	const Floats& x_steps_;
	const Floats& y_steps_;
	const float y_representation_size_;
};

template <typename OutputIt>
OutputIt HeatmapNormalizer::BatchLerp(
		const std::pair<float, float> bound_coords,
		const std::pair<float, float> values, float start_coord,
		const float step_size, OutputIt d_first) {
	auto batch_size =
			CalculateBatchSize(bound_coords.second, start_coord, step_size);
	static int i = 0;
	for (; batch_size > 0; --batch_size) {
		// do not interpolate if last value is the right bound
		// if (std::abs(start_coord - bound_coords.second) < eps)
		// 	return d_first;

		// by STL style
		LOG_TRACE("{}: bound_coords: {{{}, {}}}, start_coord: {}", ++i, bound_coords.first, bound_coords.second, start_coord);
		*d_first++ = Lerp(bound_coords, values, start_coord);
		start_coord += step_size;
	}
	return d_first;
}

template <typename OutputIt>
OutputIt HeatmapNormalizer::InterpolateRow(std::span<const float> row,
                                           const Floats& steps, float step_size,
                                           OutputIt d_first) {
	float new_coord                = 0.0f;

	std::pair<float, float> bounds = {0.0f, 0.0f};

	auto row_iter = row.begin();

	// First value is interpolating as leftmost value in the row
	// *d_first++ = *row_iter;
	// Iterate through pairs of old nodes
	for (auto step : steps) {
		// Make left bounds as old_right_bounds and
		// right bound as old_right_bound + step
		bounds = {bounds.second, bounds.second + step};

		const std::pair<float, float> values = {*row_iter, *++row_iter};
		// assert(row_iter != row.end());

		d_first = BatchLerp(bounds, values, new_coord, step_size, d_first);
		new_coord += step_size * static_cast<float>(CalculateBatchSize(
																 bounds.second, new_coord, step_size));
	}
	return d_first;
}
