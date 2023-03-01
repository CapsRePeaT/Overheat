#define pragma once

#include <cinolib/geometry/vec_mat.h>
#include <vector>
namespace MeshProcessor {
using namespace cinolib;

class BoxBoundaryRingsGenerator {
 public:
	BoxBoundaryRingsGenerator() = default;
	BoxBoundaryRingsGenerator(vec3d min_point, vec3d max_point, size_t count);

	std::vector<vec3d> xy_ring() { return xy_; }
	std::vector<vec3d> xy_z_ring() { return xy_z_; }
	std::vector<vec3d> xz_ring() { return xz_; }
	std::vector<vec3d> xz_y_ring() { return xz_y_; }
	std::vector<vec3d> yz_ring() { return yz_; }
	std::vector<vec3d> yz_x_ring() { return yz_x_; }

 private:
	std::vector<vec3d> xy_;
	std::vector<vec3d> xy_z_;
	std::vector<vec3d> xz_;
	std::vector<vec3d> xz_y_;
	std::vector<vec3d> yz_;
	std::vector<vec3d> yz_x_;
};

}  // namespace MeshProcessor