#pragma once

#include <fmt/format.h>

struct Vec2D {
	int x;
	int y;
};

struct Vec3D {
	int x;
	int y;
	int z;
};

template <>
struct fmt::formatter<Vec2D> {
	static constexpr auto parse(format_parse_context& ctx) -> decltype(ctx.begin()) {
		return ctx.end();
	}
	template <typename FormatContext>
	auto format(const Vec2D& v, FormatContext& ctx) -> decltype(ctx.out()) {
		// ctx.out() is an output iterator to write to.
		return format_to(ctx.out(), "{{x: {}, y: {}}}", v.x, v.y);
	}
};

template <>
struct fmt::formatter<Vec3D> {
	static constexpr auto parse(format_parse_context& ctx) -> decltype(ctx.begin()) {
		return ctx.end();
	}
	template <typename FormatContext>
	auto format(const Vec3D& v, FormatContext& ctx) -> decltype(ctx.out()) {
		// ctx.out() is an output iterator to write to.
		return format_to(ctx.out(), "{{x: {}, y: {}, z: {}}}", v.x, v.y, v.z);
	}
};
