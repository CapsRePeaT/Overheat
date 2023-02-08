#include "temperature_bar.h"

#include "renderer/renderer_api.h"
#include "renderer/vertexbuffer.h"
#include "renderer/vertexbufferlayout.h"

namespace renderer {

struct UiVertex {
	glm::vec2 position;
	glm::vec2 uv_coordinates;
};

TemperatureBar::TemperatureBar(float width, float height, glm::vec2 location) {
	// 0----1
	// | \  |
	// |  \ |
	// 2----3
	const std::array<const UiVertex, 4> vertices = {
			UiVertex{{location.x,         location.y}, {0, 1}},                   // 0
			UiVertex{{location.x + width, location.y}, {1, 1}},           // 1
			UiVertex{{location.x,         location.y + height}, {0, 0}},          // 2
			UiVertex{{location.x + width, location.y + height}, {0, 0}},  // 3
	};

	constexpr std::array<uint32_t, 3 * 2 * 1> raw_ibo = {2, 3, 0, 0, 3, 1};
	auto&& layout = std::make_unique<VertexBufferLayout>();
	layout->Push<float>(2);
	layout->Push<float>(2);
	auto& factory = RendererAPI::factory();
	auto&& vbo    = factory.NewVertexBuffer(vertices, std::move(layout));
	auto&& ibo    = factory.NewIndexBuffer(raw_ibo);
	vao_          = factory.NewVertexArray(std::move(vbo), std::move(ibo));
}

}  // namespace renderer