#pragma once

#include <fmt/format.h>

namespace renderer {

struct Vec2D {
	int x;
	int y;
};

struct Vec3D {
	int x;
	int y;
	int z;
};

}  // namespace renderer

template <>
struct fmt::formatter<renderer::Vec2D> {
	static constexpr auto parse(format_parse_context& ctx)
			-> decltype(ctx.begin()) {
		return ctx.end();
	}
	template <typename FormatContext>
	auto format(const renderer::Vec2D& v, FormatContext& ctx)
			-> decltype(ctx.out()) {
		// ctx.out() is an output iterator to write to.
		return format_to(ctx.out(), "{{x: {}, y: {}}}", v.x, v.y);
	}
};

template <>
struct fmt::formatter<renderer::Vec3D> {
	static constexpr auto parse(format_parse_context& ctx)
			-> decltype(ctx.begin()) {
		return ctx.end();
	}
	template <typename FormatContext>
	auto format(const renderer::Vec3D& v, FormatContext& ctx)
			-> decltype(ctx.out()) {
		// ctx.out() is an output iterator to write to.
		return format_to(ctx.out(), "{{x: {}, y: {}, z: {}}}", v.x, v.y, v.z);
	}
};
