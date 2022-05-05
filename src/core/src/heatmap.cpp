#include "heatmap.h"
#include <iostream>
#include <iomanip>

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


void Heatmap::DebugPrint(size_t step) const {
	std::cout << "x_resolution: " << x_resolution_ << " y_resolution: " <<  y_resolution_ << '\n';
	for (size_t i = 0; i < y_resolution_; i += step) {
		auto r = row(i);
		for (size_t j = 0; j < r.size(); j += step){
			std::cout << std::setprecision(3) << r[j] << '\t';
		}
		std::cout << '\n';
	}
	if (temperatures_.size() > x_resolution_ * y_resolution_) {
		std::cout << "Extra: " << '\n';
		for (size_t i = x_resolution_ * y_resolution_; i < temperatures_.size(); ++i) {
			std::cout << std::setprecision(3) << temperatures_[i] << '\t';
		}
		std::cout << '\n';
	}
}
