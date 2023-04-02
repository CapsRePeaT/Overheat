#include "temperature_bar.h"

#include "renderer/renderer_api.h"
#include "renderer/vertexbuffer.h"
#include "renderer/vertexbufferlayout.h"

namespace renderer {

struct UiVertex {
	glm::vec2 position;
	glm::vec2 uv_coordinates;
};

TemperatureBar::TemperatureBar(glm::vec2 size, glm::vec2 position,
                               std::shared_ptr<Font> font, const float min_temp, const float max_temp)
		: size_(size), position_(position), font_(std::move(font)), temperature_range_({min_temp, max_temp}) {
	vao_    = InitVao();
	InitLabels();
}

void TemperatureBar::SetTemperatureRange(float min, float max) {
	temperature_range_ = {min, max};
	InitLabels();
}

std::unique_ptr<VertexArray> TemperatureBar::InitVao() {
	// 0----1
	// | \  |
	// |  \ |
	// 2----3
	const std::array<const UiVertex, 4> vertices = {
			UiVertex{{position_.x, position_.y}, {0, 1}},                      // 0
			UiVertex{{position_.x + size_.x, position_.y}, {1, 1}},            // 1
			UiVertex{{position_.x, position_.y + size_.y}, {0, 0}},            // 2
			UiVertex{{position_.x + size_.x, position_.y + size_.y}, {0, 0}},  // 3
	};

	constexpr std::array<uint32_t, 3 * 2 * 1> raw_ibo = {2, 3, 0, 0, 3, 1};
	auto&& layout = std::make_unique<VertexBufferLayout>();
	layout->Push<float>(2);
	layout->Push<float>(2);
	auto& factory = RendererAPI::factory();
	std::vector<std::unique_ptr<VertexBuffer>> vbos;
	auto&& vbo    = factory.NewVertexBuffer(vertices, std::move(layout));
	vbos.emplace_back(std::move(vbo));
	auto&& ibo    = factory.NewIndexBuffer(raw_ibo);
	return factory.NewVertexArray(std::move(vbos), std::move(ibo));
}

TemperatureBar::Labels TemperatureBar::InitLabels() {
	labels_.clear();
	labels_.reserve(2);
	constexpr float y_text_shift = 4.0f;
	labels_.emplace_back(font_->CreateText(
			temperature_range_.first,
			position_ + glm::vec2{size_.x + 5.0f, y_text_shift}, true));
	labels_.emplace_back(font_->CreateText(
			temperature_range_.second,
			position_ + glm::vec2{size_.x + 5.0f, size_.y + y_text_shift}, true));
	return labels_;
}

}  // namespace renderer
