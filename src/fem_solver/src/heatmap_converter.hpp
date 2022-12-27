#pragma once

#include "../../core/include/databases.h"
#include "solver_db.hpp"

class HeatmapConverter {
 public:
	HeatmapConverter()     = default;
	virtual ~HeatmapConverter() = default;
	void ConvertHeatmap(FileRepresentation& file_rep,
	                    const SolverHeatmap& heatmap);
};

