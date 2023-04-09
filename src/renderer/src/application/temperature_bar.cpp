#include "temperature_bar.h"

#include "renderer/renderer_api.h"
#include "renderer/vertexbuffer.h"
#include "renderer/vertexbufferlayout.h"
#include "temperature_bar_material.h"

namespace renderer {

// TODO: make materials api-independent
struct TemperatureBar::Impl {
	std::unique_ptr<TemperatureBarMaterial> material;
};

void TemperatureBar::InitMaterial() {
	if (!impl_->material)
		impl_->material = std::make_unique<TemperatureBarMaterial>();
}

struct UiVertex {
	glm::vec2 position;
	glm::vec2 uv_coordinates;
};

TemperatureBar::TemperatureBar(glm::vec2 size, glm::vec2 position,
                               std::shared_ptr<Font> font, const float min_temp,
                               const float max_temp)
		: size_(size),
			position_(position),
			font_(std::move(font)),
			temperature_range_({min_temp, max_temp}),
			impl_(std::make_unique<Impl>()) {
	vao_ = InitVao();
	InitLabels();
}
TemperatureBar::~TemperatureBar() = default;

void TemperatureBar::SetTemperatureRange(float min, float max) {
	temperature_range_ = {min, max};
	InitLabels();
}

void TemperatureBar::SetColorRange(glm::vec3 min, glm::vec3 max) {
	InitMaterial();
	impl_->material->SetColorRange(min, max);
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
	std::vector<std::shared_ptr<VertexBuffer>> vbos;
	auto&& vbo = factory.NewVertexBuffer(vertices, std::move(layout));
	vbos.emplace_back(std::move(vbo));
	auto&& ibo = factory.NewIndexBuffer(raw_ibo);
	return factory.NewVertexArray(std::move(vbos), std::move(ibo));
}

TemperatureBar::Labels TemperatureBar::InitLabels() {
	constexpr size_t labels_count = 5;
	labels_.clear();
	labels_.reserve(labels_count);
	// magics
	constexpr float y_text_shift = 4.0f;
	const float x_local_position = size_.x + 5.0f;

	constexpr std::array<float, labels_count> ts = {0.0f, 0.25f, 0.5f, 0.75f,
	                                                1.0f};
	for (auto t : ts) {
		const auto temp =
				std::lerp(temperature_range_.first, temperature_range_.second, t);
		const glm::vec2 local_position = {
				x_local_position, std::lerp(size_.y, 0.0f, t) + y_text_shift};

		labels_.emplace_back(
				font_->CreateText(temp, position_ + local_position, true));
	}

	return labels_;
}

bool TemperatureBar::SetContextForDraw(RendererContext& ctx) {
	InitMaterial();
	ctx.SetVao(vertex_array());
	// maybe callbacks is not the best solution?..
	ctx.SetMaterialCallbacks(
			[&material = *impl_->material, &ctx]() {
				material.Use(ctx.camera().uiViewMatrix());
			},
			[&material = *impl_->material]() { material.Unuse(); });
	return true;
}

}  // namespace renderer
