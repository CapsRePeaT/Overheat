#include "box_mesh.h"

#include <iostream>

#include "../src/utils.h"

namespace MeshProcessor {
using namespace cinolib;
BoxMesh::BoxMesh(const std::pair<float, float>& x1_x2_points,
                 const std::pair<float, float>& y1_y2_points,
                 const std::pair<float, float>& z1_z2_points, size_t i_layer) {
	min_point = vec3d{x1_x2_points.first, y1_y2_points.first, z1_z2_points.first};
	max_point =
			vec3d{x1_x2_points.second, y1_y2_points.second, z1_z2_points.second};
	generator = BoxBoundaryRingsGenerator(min_point, max_point, 10);
	layer = i_layer;
}

void BoxMesh::calculate_mesh() {
	auto diff = max_point - min_point;
	auto length = diff.x();
	auto width = diff.y();
	auto height = diff.z();
	static int counter = 0;
	std::cout << counter << std::endl;
	// xy, xy_z triangulations
	auto xy_poly = make_multi_polygon(generator.xy_ring(), holes_lower);
	xy = trimeshFromPoly(xy_poly);

	auto xy_z_poly = make_multi_polygon(generator.xy_z_ring(), holes_upper);
	if (counter == 2) {
		std::vector<vec3d> verts;
		for (auto& vert : generator.xy_z_ring()) verts.push_back(vert);
		for (auto& hole : holes_upper)
			for (auto& vert : hole) verts.push_back(vert);
		std::vector<uint> polys;
		DrawableTrimesh<> m(verts, polys);
		m.vert_set_color(Color::RED());
		m.show_mesh_points();
		m.vert_set_alpha(1.0f);
		m.updateGL();
		GLcanvas gui;
		gui.push(&m);

		DrawableArrow x(vec3d(-10, 0, 0), vec3d(10, 0, 0));
		x.color = Color::GREEN();
		x.size = 0.1;
		DrawableArrow y(vec3d(0, -10, 0), vec3d(0, 10, 0));
		y.color = Color::BLUE();
		y.size = 0.1;
		DrawableArrow z(vec3d(0, 0, -10), vec3d(0, 0, 10));
		z.size = 0.1;

		gui.push(&x);
		gui.push(&y);
		gui.push(&z);

		gui.launch();
	}
	xy_z = trimeshFromPoly(xy_z_poly);
	xy_z.translate(vec3d(0, 0, height));

	// xz, xz_y triangulations
	auto xz_ring_rotated = rotate(generator.xz_ring(), vec3d(1, 0, 0), M_PI / 2);
	BoostPolygon xz_poly = make_polygon(xz_ring_rotated);
	xz = trimeshFromPoly(xz_poly);
	auto& mesh_xz_verts = xz.vector_verts();
	mesh_xz_verts = rotate(mesh_xz_verts, vec3d(1, 0, 0), -M_PI / 2);
	round_boundary(xz);

	xz_y = xz;
	xz_y.translate(vec3d(0, width, 0));
	round_boundary(xz_y);

	// xz, xz_y triangulations
	auto yz_ring_rotated = rotate(generator.yz_ring(), vec3d(0, 1, 0), M_PI / 2);
	BoostPolygon yz_poly = make_polygon(yz_ring_rotated);
	yz = trimeshFromPoly(yz_poly);
	auto& mesh_yz_verts = yz.vector_verts();
	mesh_yz_verts = rotate(mesh_yz_verts, vec3d(0, 1, 0), -M_PI / 2);
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
