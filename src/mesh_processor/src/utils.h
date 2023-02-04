#define pragma once

#include <cinolib/boost_polygon_wrap.h>
#include <cinolib/dual_mesh.h>
#include <cinolib/geometry/vec_mat.h>
#include <cinolib/gl/glcanvas.h>
#include <cinolib/gl/volume_mesh_controls.h>
#include <cinolib/meshes/meshes.h>
#include <cinolib/tetgen_wrap.h>
#include <cinolib/vector_serialization.h>

#include <vector>
#include <cmath>

namespace MeshProcessor {
using namespace cinolib;
// rotate cloud of points around @rot_vec for @angle
std::vector<vec3d> rotate(std::vector<vec3d> verts, vec3d rot_vec,
                          double angle);
// translate cloud of points on @translation
std::vector<vec3d> translate(std::vector<vec3d> verts, vec3d translation);

template<class T>
static T Round(T a)
{
	static_assert(std::is_floating_point<T>::value, "Round<T>: T must be floating point");

	return (a > 0) ? ::floor(a + static_cast<T>(0.5)) : ::ceil(a - static_cast<T>(0.5));
}

template<class T>
static T round_t(T a)
{
	static_assert(std::is_floating_point<T>::value, "Round<T>: T must be floating point");
	int places = 5;
	const T shift = pow(static_cast<T>(10.0), places);

	return std::abs(Round(a * shift) / shift);
}

vec3d round_vec3d(auto vec);

void round_boundary(DrawableTrimesh<>& mesh);

BoostPolygon make_multi_polygon(
		const std::vector<vec3d>& outer_ring,
		const std::vector<std::vector<vec3d>>& holes = {});

template <typename Poly>
DrawableTrimesh<> trimeshFromPoly(const Poly& poly) {
	std::vector<vec3d> verts;
	std::vector<uint> tris;
	char opt[100];
	sprintf(opt, "qca%f", 50000.f);
	triangulate_polygon(poly, std::string(opt), 0, verts, tris);
	DrawableTrimesh<> ret(verts, tris);
	return ret;
}

}  // namespace MeshProcessor