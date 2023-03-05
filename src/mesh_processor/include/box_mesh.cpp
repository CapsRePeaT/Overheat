#include "box_mesh.h"

#include <cinolib/drawable_sphere.h>
#include <cinolib/gl/draw_sphere.h>

#include "../../common/include/common.h"
#include "../src/utils.h"

namespace MeshProcessor {

using namespace cinolib;
void round_boundary(DrawableTrimesh<>& mesh) {
	auto round_vec3d = [](const auto vec) {
		vec3d ret;
		ret.x() = std::round(std::abs(vec.x()) * DefaultRoundBoundary) /
		          DefaultRoundBoundary;
		ret.y() = std::round(std::abs(vec.y()) * DefaultRoundBoundary) /
		          DefaultRoundBoundary;
		ret.z() = std::round(std::abs(vec.z()) * DefaultRoundBoundary) /
		          DefaultRoundBoundary;
		return ret;
	};

	auto& verts = mesh.vector_verts();
	for (auto vid : mesh.get_boundary_vertices()) {
		verts[vid] = round_vec3d(verts[vid]);
	}
}

BoxMesh::BoxMesh(const std::pair<float, float>& x1_x2_points,
                 const std::pair<float, float>& y1_y2_points,
                 const std::pair<float, float>& z1_z2_points,
                 const size_t i_layer, MeshConstraintFunction area_constraint,
                 const std::optional<double> step) {
	min_point =
			vec3d{round_float(x1_x2_points.first), round_float(y1_y2_points.first),
	          round_float(z1_z2_points.first)};
	max_point =
			vec3d{round_float(x1_x2_points.second), round_float(y1_y2_points.second),
	          round_float(z1_z2_points.second)};

	generator   = BoxBoundaryRingsGenerator(min_point, max_point, step);
	area_thresh = area_constraint(generator.min_segment());
	layer       = i_layer;
}

void BoxMesh::calculate_mesh() {
	const auto diff   = max_point - min_point;
	const auto length = diff.x();
	const auto width  = diff.y();
	const auto height = diff.z();

	static int counter = 0;
	std::cout << counter << std::endl;
	auto xy_poly = make_multi_polygon(generator.xy_ring(), holes_lower);
	xy           = trimeshFromPoly(xy_poly, area_thresh);
	round_boundary(xy);

	auto xy_z_poly = make_multi_polygon(generator.xy_z_ring(), holes_upper);
	xy_z           = trimeshFromPoly(xy_z_poly, area_thresh);
	xy_z.translate(vec3d(0, 0, height));
	round_boundary(xy_z);

	// xz, xz_y triangulations
	auto xz_ring_rotated = rotate(generator.xz_ring(), vec3d(1, 0, 0), M_PI / 2);
	BoostPolygon xz_poly = make_polygon(xz_ring_rotated);
	xz                   = trimeshFromPoly(xz_poly, area_thresh);
	auto& mesh_xz_verts  = xz.vector_verts();
	mesh_xz_verts        = rotate(mesh_xz_verts, vec3d(1, 0, 0), -M_PI / 2);
	round_boundary(xz);
	xz_y = xz;
	xz_y.translate(vec3d(0, width, 0));
	round_boundary(xz_y);

	// xz, xz_y triangulations
	auto yz_ring_rotated = rotate(generator.yz_ring(), vec3d(0, 1, 0), M_PI / 2);
	BoostPolygon yz_poly = make_polygon(yz_ring_rotated);
	yz                   = trimeshFromPoly(yz_poly, area_thresh);
	auto& mesh_yz_verts  = yz.vector_verts();
	mesh_yz_verts        = rotate(mesh_yz_verts, vec3d(0, 1, 0), -M_PI / 2);
	round_boundary(yz);

	yz_x = yz;
	yz_x.translate(vec3d(length, 0, 0));
	round_boundary(yz_x);
	++counter;
}

void BoxMesh::translate_to_origin() {
	xy_z.translate(min_point);
	xy.translate(min_point);
	xz.translate(min_point);
	xz_y.translate(min_point);
	yz.translate(min_point);
	yz_x.translate(min_point);
}

void BoxMesh::merge_meshes() {
	for (auto& mesh : {xy_z, xy, xz, xz_y, yz, yz_x}) total_mesh += mesh;
}
}  // namespace MeshProcessor
