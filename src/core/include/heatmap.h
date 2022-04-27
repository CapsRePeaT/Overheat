#pragma once

#include <limits>
#include <span>
#include <vector>

#include "common.h"

class Heatmap {
 public:
	explicit Heatmap(Floats temperature_mv, size_t x_resolution,
	                 size_t y_resolution);
	// TBD: <some_type> operator[](std::size_t idx); ?
	[[nodiscard]] inline std::span<const float> row(size_t i) const {
		return {temperatures_.begin() + i * x_resolution_, x_resolution_};
	}
	[[nodiscard]] inline auto temperatures() const { return temperatures_; }
	[[nodiscard]] inline size_t x_resolution() const { return x_resolution_; }
	[[nodiscard]] inline size_t y_resolution() const { return y_resolution_; }
	[[nodiscard]] inline float min_temp() const { return min_temp_; }
	[[nodiscard]] inline float max_temp() const { return max_temp_; }

 private:
	// for alexey purposes we want to use single mem array, so we implement 2d
	// array access on out own
	const Floats temperatures_;
	const size_t x_resolution_;
	const size_t y_resolution_;
	float max_temp_ = std::numeric_limits<float>::min();
	float min_temp_ = std::numeric_limits<float>::max();
};

using Heatmaps = std::vector<Heatmap>;
