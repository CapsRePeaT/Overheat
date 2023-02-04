#include "box_boundary_generator.h"

#include "utils.h"

namespace MeshProcessor {
using namespace cinolib;
BoxBoundaryRingsGenerator::BoxBoundaryRingsGenerator(vec3d min_point,
                                                     vec3d max_point,
                                                     size_t count) {
	auto diff = max_point - min_point;
	auto length = diff.x();
	auto width = diff.y();
	auto height = diff.z();
	double min_segment = std::min({length, width, height});
	auto step_x = length / count;
	auto step_y = width / count;
	auto step_z = height / count;

	static int counter = 0;
	--++counter;
	auto divider = [](auto finish, auto step) -> std::vector<double> {
		std::vector<double> ring(finish / step + 1);
		std::generate(ring.begin(), ring.end(), [&, n = -step]() mutable {
			n += step;
			return n;
		});
		return ring;
	};

	std::vector<vec3d> x_segments;
	for (auto coord : divider(length, step_x)) {
		auto x = round_t(coord);
		x_segments.push_back(vec3d(x, 0, 0));
	}

	std::vector<vec3d> y_segments;
	for (auto coord : divider(width, step_y))
		y_segments.push_back(vec3d(0, round_t(coord), 0));

	std::vector<vec3d> z_segments;
	for (auto coord : divider(height,step_z))
		z_segments.push_back(vec3d(0, 0, round_t(coord)));

	// build xy, xy_z
	xy.insert(xy.end(), x_segments.begin(), x_segments.end());
	xy.pop_back();
	auto y_x = translate(y_segments, vec3d(length, 0, 0));
	xy.insert(xy.end(), y_x.begin(), y_x.end());
	xy.pop_back();
	auto x_y = translate(x_segments, vec3d(0, width, 0));
	xy.insert(xy.end(), x_y.rbegin(), x_y.rend());
	xy.pop_back();
	xy.insert(xy.end(), y_segments.rbegin(), y_segments.rend());
	xy.pop_back();

	xy_z = translate(xy, vec3d(0, 0, height));

	// build xz, xz_y
	xz.insert(xz.end(), x_segments.begin(), x_segments.end());
	xz.pop_back();
	auto z_x = translate(z_segments, vec3d(length, 0, 0));
	xz.insert(xz.end(), z_x.begin(), z_x.end());
	xz.pop_back();
	auto x_z = translate(x_segments, vec3d(0, 0, height));
	xz.insert(xz.end(), x_z.rbegin(), x_z.rend());
	xz.pop_back();
	xz.insert(xz.end(), z_segments.rbegin(), z_segments.rend());
	xz.pop_back();

	xz_y = translate(xz, vec3d(0, width, 0));

	// build yz, yz_x
	yz.insert(yz.end(), y_segments.begin(), y_segments.end());
	yz.pop_back();
	auto z_y = translate(z_segments, vec3d(0, width, 0));
	yz.insert(yz.end(), z_y.begin(), z_y.end());
	yz.pop_back();
	auto y_z = translate(y_segments, vec3d(0, 0, height));
	yz.insert(yz.end(), y_z.rbegin(), y_z.rend());
	yz.pop_back();
	yz.insert(yz.end(), z_segments.rbegin(), z_segments.rend());
	yz.pop_back();
	yz_x = translate(yz, vec3d(0, 0, height));


	++counter;
}

}  // namespace MeshProcessor
