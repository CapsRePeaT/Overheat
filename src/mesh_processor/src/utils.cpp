#include "utils.h"

namespace MeshProcessor {
using namespace cinolib;
std::vector<vec3d> rotate(std::vector<vec3d> verts, vec3d rot_vec,
                          double angle) {
	for (auto& v : verts) v.rotate(rot_vec, angle);
	return verts;
}

std::vector<vec3d> translate(std::vector<vec3d> verts, vec3d translation) {
	for (auto& v : verts) {
		v.x() = v.x() + translation.x();
		v.y() = v.y() + translation.y();
		v.z() = v.z() + translation.z();
	}
	return verts;
}

BoostPolygon make_multi_polygon(const std::vector<vec3d>& outer_ring,
                                const std::vector<std::vector<vec3d>>& holes) {
	BoostPolygon poly = make_polygon(outer_ring);
	auto& inners      = poly.inners();
	for (auto& inner : holes) {
		boost::geometry::model::ring<BoostPoint> inner_ring;
		for (auto point : inner)
			boost::geometry::append(inner_ring, BoostPoint(point.x(), point.y()));
		inners.push_back(inner_ring);
	}
	boost::geometry::correct(poly);
	return poly;
}

double round_float(float x)  {
	int y              = x;
	float z            = x - y;
	float m            = 1000;
	float q            = z * m;
	return round(q) + y * m;
}
}  // namespace MeshProcessor