#pragma once

#include <optional>

#include "../../core/include/databases.h"
#include "solver_db.hpp"

using MeshConstraintFunction = std::function<double(const double)>;

/// <summary>
/// Creates Fem elements and indexation
/// </summary>
class GeometryCutter {
 public:
	/// <summary>
	/// uses corner_points_step to build corner points
	/// </summary>
	/// <param name="corner_points_step"> fem element max edge length </param>
	/// <param name="area_constraint"> fem element max side area</param>
	/// <param name="volume_constraint"> fem element max volume</param>
	GeometryCutter(double corner_points_step,
	               MeshConstraintFunction area_constraint,
	               MeshConstraintFunction volume_constraint);
	/// uses min edge of box to build corner points instead of corner_points_step
	GeometryCutter(MeshConstraintFunction area_constraint,
	               MeshConstraintFunction volume_constraint);
	~GeometryCutter() = default;
	// if show_mesh == true, it blocks main thread
	// until you close visualization window
	FsDatapack PrepareGeometry(FileRepresentation& file_rep,
	                           bool show_mesh = false);
	FsDatapack PrepareTestGeometry();
	/// test function, sets 20c to point woth index 0
	/// <returns> temperature of point by index</returns>
	std::vector<std::pair<double, size_t>> TestTempAndIndexes() {
		//TODO: just return points_temps_ after matrix refactoring;
		std::vector<std::pair<double, size_t>> result;
		result.push_back(std::make_pair(20, 0));
		return result;
	}
	/// <returns> temperature of point by index</returns>
	std::vector<std::pair<double, size_t>> TempAndIndexes() {
		//TODO: just return points_temps_ after matrix refactoring;
		std::vector<std::pair<double, size_t>> result;
		for (const auto temp_and_point : points_temps_)
			result.push_back(std::make_pair(temp_and_point.first,
				index_to_coord_map_.AddVertice(temp_and_point.second)));
		return result;
	}
	// store std::pair<double, Point3d>
	VerticeIndexes& GetVerticeIndexes();
	const VerticeIndexes& GetVerticeIndexes() const;
 private:
	std::optional<double> corner_points_step_;
	MeshConstraintFunction area_constraint_;
	MeshConstraintFunction volume_constraint_;
	VerticeIndexes index_to_coord_map_;
	std::set<std::pair<double, Point3D>> points_temps_;
};



