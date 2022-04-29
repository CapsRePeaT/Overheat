#include "heatmap.h"

#include <bits/ranges_algo.h>

Heatmap::Heatmap(Floats temperature_mv, const size_t x_resolution,
                 const size_t y_resolution,
                 const std::ranges::min_max_result<float>& temp_range)
		: temperatures_(std::move(temperature_mv)),
			x_resolution_(x_resolution),
			y_resolution_(y_resolution),
			max_temp_(temp_range.max),
			min_temp_(temp_range.min) {
	if (&temp_range == &min_max_floats) {
		auto temperature_range = std::ranges::minmax(temperatures_);

		min_temp_ = temperature_range.min;
		max_temp_ = temperature_range.max;
	}
}
