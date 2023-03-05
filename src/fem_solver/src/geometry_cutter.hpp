#pragma once

#include <optional>

#include "../../core/include/databases.h"
#include "solver_db.hpp"

using MeshConstraintFunction = std::function<double(const double)>;

class GeometryCutter {
 public:
	// uses corner_points_step to build corner points
	GeometryCutter(double corner_points_step,
	               MeshConstraintFunction area_constraint,
	               MeshConstraintFunction volume_constraint);
	// uses min edge of box to build corner points
	GeometryCutter(MeshConstraintFunction area_constraint,
	               MeshConstraintFunction volume_constraint);
	~GeometryCutter() = default;
	// if show_mesh == true, it blocks main thread
	// until you close visualization window
	FsDatapack PrepareGeometry(FileRepresentation& file_rep,
	                           bool show_mesh = false);
	VerticeIndexes& GetVerticeIndexes();
	const VerticeIndexes& GetVerticeIndexes() const;

 private:
	std::optional<double> corner_points_step_;
	MeshConstraintFunction area_constraint_;
	MeshConstraintFunction volume_constraint_;
	VerticeIndexes index_to_coord_map_;
};
