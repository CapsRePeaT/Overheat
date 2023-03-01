#pragma once

#include "../../core/include/databases.h"
#include "solver_db.hpp"

class GeometryCutter {
 public:
	GeometryCutter(double area_thresh, double volume_thresh);
	~GeometryCutter() = default;
	// if show_mesh == true, it blocks main thread
	// until you close visualization window
	FsDatapack PrepareGeometry(FileRepresentation& file_rep,
	                           bool show_mesh = false);
	VerticeIndexes& GetVerticeIndexes();
	const VerticeIndexes& GetVerticeIndexes() const;

 private:
	double area_thresh_;
	double volume_thresh_;
	VerticeIndexes index_to_coord_map_;
};
