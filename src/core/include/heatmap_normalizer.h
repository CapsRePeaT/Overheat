#pragma once

#include "common.h"
#include "databases.h"
#include "heatmap.h"

class HeatmapNormalizer {
 public:
	HeatmapNormalizer(float min_step, float x_size, float y_size,
	                  size_t max_resolution, const Floats& x_steps,
	                  const Floats& y_steps);
	Floats Normalize(const Heatmap& heatmap);

 private:
	const size_t resolution_;
	const float x_new_step_;
	const float y_new_step_;
	const Floats& x_steps_;
	const Floats& y_steps_;
};
