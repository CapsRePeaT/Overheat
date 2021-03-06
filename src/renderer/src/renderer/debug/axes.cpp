#include "axes.h"

#include <array>
#include <memory>

#include "renderer/renderer_api.h"
#include "renderer/scene_object.h"
#include "renderer/vertexbufferlayout.h"

namespace renderer::debug {
namespace {

struct Vertex {
	glm::vec3 position;
	glm::vec3 color;
};

}  // namespace

Axes::Axes() {
	//
	//   ^ z (blue)
	//   |  ^ y (green)
	//   | /
	//   |/
	//   o------> x (red)
	//
	static constexpr std::array<const Vertex, 6> vertices = {
			//        x     y     z       r     g     b
			Vertex({{0.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}}),  // x1
			Vertex({{1.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}}),  // x2
			Vertex({{0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}}),  // y1
			Vertex({{0.0f, 1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}}),  // y2
			Vertex({{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}}),  // z1
			Vertex({{0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f}}),  // z2
	};

	static constexpr std::array<uint32_t, 3 * 2> raw_ibo = {0, 1, 2, 3, 4, 5};

	auto layout = std::make_unique<VertexBufferLayout>();
	layout->Push<float>(3);
	layout->Push<float>(3);
	auto& factory = RendererAPI::instance().factory();
	auto&& vbo    = factory.NewVertexBuffer(vertices, std::move(layout));
	auto&& ibo    = factory.NewIndexBuffer(raw_ibo);
	vao_          = factory.NewVertexArray(std::move(vbo), std::move(ibo));
}

}  // namespace renderer::debug
