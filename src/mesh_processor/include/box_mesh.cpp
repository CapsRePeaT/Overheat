#include "box_mesh.h"

#include <iostream>

#include "../src/utils.h"
#include <cinolib/gl/draw_sphere.h>
#include <cinolib/drawable_sphere.h>

double round_float(float x){
	int y=x;
	float z=x-y;
	float m=1000;
	float q=z*m;
	double r=round(q)+y*m;
	return r;
}

namespace MeshProcessor {
using namespace cinolib;
BoxMesh::BoxMesh(const std::pair<float, float>& x1_x2_points,
                 const std::pair<float, float>& y1_y2_points,
                 const std::pair<float, float>& z1_z2_points, size_t i_layer) {
	round_float(x1_x2_points.first);
	min_point = vec3d{round_float(x1_x2_points.first), round_float(y1_y2_points.first),
	                  round_float(z1_z2_points.first)};
	max_point = vec3d{round_float(x1_x2_points.second), round_float(y1_y2_points.second),
	                  round_float(z1_z2_points.second)};
	generator = BoxBoundaryRingsGenerator(min_point, max_point, 10);
	layer = i_layer;
}

template<typename T>
std::set<T> findDuplicates(std::vector<T> vec)        // no-ref, no-const
{
	std::set<int> duplicates;
	std::sort(vec.begin(), vec.end());
	std::set<int> distinct(vec.begin(), vec.end());
	std::set_difference(vec.begin(), vec.end(), distinct.begin(), distinct.end(),
	                    std::inserter(duplicates, duplicates.end()));
	return duplicates;
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
	xy_z = trimeshFromPoly(xy_z_poly);
	xy_z.translate(vec3d(0, 0, height));

	// xz, xz_y triangulations
	auto xz_ring_rotated = rotate(generator.xz_ring(), vec3d(1, 0, 0), M_PI / 2);
	BoostPolygon xz_poly = make_polygon(xz_ring_rotated);
	xz = trimeshFromPoly(xz_poly);
	auto& mesh_xz_verts = xz.vector_verts();
	mesh_xz_verts = rotate(mesh_xz_verts, vec3d(1, 0, 0), -M_PI / 2);

	xz_y = xz;
	xz_y.translate(vec3d(0, width, 0));

	// xz, xz_y triangulations
	auto yz_ring_rotated = rotate(generator.yz_ring(), vec3d(0, 1, 0), M_PI / 2);
	BoostPolygon yz_poly = make_polygon(yz_ring_rotated);
	yz = trimeshFromPoly(yz_poly);
	auto& mesh_yz_verts = yz.vector_verts();
	mesh_yz_verts = rotate(mesh_yz_verts, vec3d(0, 1, 0), -M_PI / 2);


	yz_x = yz;
	yz_x.translate(vec3d(length, 0, 0));

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
