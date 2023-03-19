#define pragma once

#include <cinolib/geometry/vec_mat.h>
#include <cinolib/meshes/meshes.h>

#include <vector>

#include "../../fem_solver/src/geometry_cutter.hpp"
#include "../src/box_boundary_generator.h"

namespace MeshProcessor {
using namespace cinolib;
struct BoxMesh {
	BoxMesh(const std::pair<float, float>& x1_x2_points,
	        const std::pair<float, float>& y1_y2_points,
	        const std::pair<float, float>& z1_z2_points, size_t i_layer,
	        MeshConstraintFunction area_constraint,
	        const std::optional<double> step);

	void calculate_mesh();
	void translate_to_origin();
	void merge_meshes();

	int layer;
	std::vector<size_t> boxes_upper{};
	std::vector<size_t> boxes_lower{};

	DrawableTrimesh<> xy;
	DrawableTrimesh<> xy_z;
	DrawableTrimesh<> xz;
	DrawableTrimesh<> xz_y;
	DrawableTrimesh<> yz;
	DrawableTrimesh<> yz_x;
	DrawableTrimesh<> total_mesh;
	double area_thresh;
	double corner_points_step;

	std::vector<std::vector<vec3d>> holes_upper = {};
	std::vector<std::vector<vec3d>> holes_lower = {};

	vec3d min_point;
	vec3d max_point;

	BoxBoundaryRingsGenerator generator;
};
}  // namespace MeshProcessor