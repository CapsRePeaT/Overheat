#include "heatmap.h"

#include <utility>

Heatmap::Heatmap(Floats temperature_mv, size_t x_resolution,
                 size_t y_resolution)
		: temperatures_(std::move(temperature_mv)),
			x_resolution_(x_resolution),
			y_resolution_(y_resolution) {}
