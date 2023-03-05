#define pragma once

#include <cinolib/geometry/vec_mat.h>

#include <optional>
#include <vector>
namespace MeshProcessor {
using namespace cinolib;

class BoxBoundaryRingsGenerator {
 public:
	BoxBoundaryRingsGenerator() = default;
	BoxBoundaryRingsGenerator(vec3d min_point, vec3d max_point,
	                          const std::optional<double> step_opt);

	double min_segment() const { return min_segment_; }
	std::vector<vec3d> xy_ring() const { return xy_; }
	std::vector<vec3d> xy_z_ring() const { return xy_z_; }
	std::vector<vec3d> xz_ring() const { return xz_; }
	std::vector<vec3d> xz_y_ring() const { return xz_y_; }
	std::vector<vec3d> yz_ring() const { return yz_; }
	std::vector<vec3d> yz_x_ring() const { return yz_x_; }

 private:
	double min_segment_;
	std::vector<vec3d> xy_;
	std::vector<vec3d> xy_z_;
	std::vector<vec3d> xz_;
	std::vector<vec3d> xz_y_;
	std::vector<vec3d> yz_;
	std::vector<vec3d> yz_x_;
};

}  // namespace MeshProcessor