#include "box_boundary_generator.h"

#include "utils.h"

namespace MeshProcessor {
using namespace cinolib;
BoxBoundaryRingsGenerator::BoxBoundaryRingsGenerator(
		vec3d min_point, vec3d max_point, const std::optional<double> step_opt) {
	const auto diff   = max_point - min_point;
	const auto length = diff.x();
	const auto width  = diff.y();
	const auto height = diff.z();
	const auto step =
			step_opt.has_value() ? *step_opt : std::min({length, width, height});
	min_segment_ = step;

	auto divider = [&](auto finish) -> std::vector<double> {
		std::vector<double> ring(finish / step + 1);
		if (step > finish)
			ring.push_back(0);
		std::generate(ring.begin(), ring.end(), [&, n = -step]() mutable {
			n += step;
			return n;
		});
		return ring;
	};

	auto generate_segments = [&](const auto final_point, const auto segment,
	                             auto point_builder) {
		std::vector<vec3d> segments;
		for (auto coord : divider(segment))
			segments.push_back(point_builder(coord));
		if (segments.back() != final_point) {
			segments.pop_back();
			segments.push_back(final_point);
		}
		return segments;
	};

	auto x_segments = generate_segments(
			vec3d{length, 0, 0}, length,
			[](const auto coord) { return vec3d(round_t(coord), 0, 0); });

	auto y_segments = generate_segments(
			vec3d{0, width, 0}, width,
			[](const auto coord) { return vec3d(0, round_t(coord), 0); });

	auto z_segments = generate_segments(
			vec3d{0, 0, height}, height,
			[](const auto coord) { return vec3d(0, 0, round_t(coord)); });

	auto build_frame = [](const auto& segment1, const auto& segment2,
	                      const auto segment1_offset,
	                      const auto segment2_offset) {
		std::vector<vec3d> target_ring;
		target_ring.insert(target_ring.end(), segment1.begin(), segment1.end());
		target_ring.pop_back();
		auto right_bound = translate(segment2, segment2_offset);
		target_ring.insert(target_ring.end(), right_bound.begin(),
		                   right_bound.end());
		target_ring.pop_back();
		auto left_bound = translate(segment1, segment1_offset);
		target_ring.insert(target_ring.end(), left_bound.rbegin(),
		                   left_bound.rend());
		target_ring.pop_back();
		target_ring.insert(target_ring.end(), segment2.rbegin(), segment2.rend());
		target_ring.pop_back();
		return target_ring;
	};

	// build xy, xy_z
	xy_   = build_frame(x_segments, y_segments, vec3d(0, width, 0),
	                    vec3d(length, 0, 0));
	xy_z_ = translate(xy_, vec3d(0, 0, height));

	// build xz, xz_y
	xz_   = build_frame(x_segments, z_segments, vec3d(0, 0, height),
	                    vec3d(length, 0, 0));
	xz_y_ = translate(xz_, vec3d(0, width, 0));

	// build yz, yz_x
	yz_   = build_frame(y_segments, z_segments, vec3d(0, 0, height),
	                    vec3d(0, width, 0));
	yz_x_ = translate(yz_, vec3d(0, 0, height));
}

}  // namespace MeshProcessor
