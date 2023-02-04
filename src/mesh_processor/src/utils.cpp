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


vec3d round_vec3d(auto vec) {
	vec3d ret;
	ret.x() = std::round(vec.x() * 100000.0) / 100000.0;
	ret.y() = std::round(vec.y() * 100000.0) / 100000.0;
	ret.z() = std::round(vec.z() * 100000.0) / 100000.0;
	return ret;
};


BoostPolygon make_multi_polygon(const std::vector<vec3d>& outer_ring,
                                const std::vector<std::vector<vec3d>>& holes) {
	BoostPolygon poly = make_polygon(outer_ring);
	auto& inners = poly.inners();
	for (auto& inner : holes) {
		boost::geometry::model::ring<BoostPoint> inner_ring;
		for (auto point : inner)
			boost::geometry::append(inner_ring, BoostPoint(point.x(), point.y()));
		inners.push_back(inner_ring);
	}
	boost::geometry::correct(poly);
	return poly;
}

}  // namespace MeshProcessor