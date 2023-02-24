#pragma once

#include "../../core/include/databases.h"
#include "solver_db.hpp"

class GeometryCutter {
 public:
	GeometryCutter(double area_thresh,
	               double volume_thresh);
	~GeometryCutter() = default;
	FsDatapack PrepareGeometry(FileRepresentation& file_rep);
	VerticeIndexes& GetVerticeIndexes();
	const VerticeIndexes& GetVerticeIndexes() const;

 private:
	double area_thresh_;
	double volume_thresh_;
	VerticeIndexes index_to_coord_map_;
};
