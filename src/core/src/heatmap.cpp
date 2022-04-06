#include "heatmap.h"

#include <utility>

Heatmap::Heatmap(std::vector<float> temperature_mv)
		: temperature_(std::move(temperature_mv)) {}
