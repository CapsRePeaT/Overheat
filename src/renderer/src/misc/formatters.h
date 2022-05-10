#include <fmt/format.h>
#include <glm/detail/qualifier.hpp>

template <glm::length_t L, typename T, glm::qualifier Q>
struct fmt::formatter<glm::vec<L, T, Q>> {
	static constexpr auto parse(format_parse_context& ctx)
			-> decltype(ctx.begin()) {
		return ctx.end();
	}
	template <typename FormatContext>
	auto format(const glm::vec<L, T, Q>& v, FormatContext& ctx)
			-> decltype(ctx.out()) {
		constexpr std::array<const char*, 4> vars = {"x", "y", "z", "w"};
		auto out = format_to(ctx.out(), "vec{{");
		for (glm::length_t i = 0; i < L; ++i) {
			if (i != 0)
				out = format_to(out, ", ");
			out = format_to(out, "{}: {}", vars[i], v[i]);
		}
		return format_to(out, "}}");
	}
};
