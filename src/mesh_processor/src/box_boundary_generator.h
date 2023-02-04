#define pragma once
#include <cinolib/geometry/vec_mat.h>

#include <vector>
namespace MeshProcessor {
using namespace cinolib;

class BoxBoundaryRingsGenerator {
 public:
	BoxBoundaryRingsGenerator() = default;
	BoxBoundaryRingsGenerator(vec3d min_point, vec3d max_point, size_t count);

	std::vector<vec3d> xy_ring() { return xy; }
	std::vector<vec3d> xy_z_ring() { return xy_z; }
	std::vector<vec3d> xz_ring() { return xz; }
	std::vector<vec3d> xz_y_ring() { return xz_y; }
	std::vector<vec3d> yz_ring() { return yz; }
	std::vector<vec3d> yz_x_ring() { return yz_x; }

 private:
	std::vector<vec3d> xy;
	std::vector<vec3d> xy_z;
	std::vector<vec3d> xz;
	std::vector<vec3d> xz_y;
	std::vector<vec3d> yz;
	std::vector<vec3d> yz_x;
};

}  // namespace MeshProcessor