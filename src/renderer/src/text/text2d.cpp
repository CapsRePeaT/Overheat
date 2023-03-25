#include "text2d.h"

#include "renderer/renderer_api.h"

namespace renderer {

// TODO: write straight to opengl buffer
void Text2D::BeginPlacing(std::u32string text, glm::vec2 position,
                                         const bool align_to_pixels) {
	text_ = std::move(text);
	position_ = position;
	align_to_pixels_ = align_to_pixels;
	character_indices_.clear();
	characters_vertices_.clear();
	vao_.reset();
}

void Text2D::AddChar(CharacterVertex top_left, CharacterVertex bottom_right) {
	CharacterVertex top_right({bottom_right.pos.x, top_left.pos.y},
	                          {bottom_right.uv.s, top_left.uv.t});
	CharacterVertex bottom_left({top_left.pos.x, bottom_right.pos.y},
	                            {top_left.uv.s, bottom_right.uv.t});
	uint32_t shift = 4u * characters_vertices_.size();

	// 0----1
	// |  / |
	// | /  |
	// 3----2
	glm::u32vec3 triangle1 = character_index_buffer_base1 + shift;
	glm::u32vec3 triangle2 = character_index_buffer_base2 + shift;
	character_indices_.push_back(triangle1);
	character_indices_.push_back(triangle2);
	characters_vertices_.push_back(
			{{top_left, top_right, bottom_right, bottom_left}});
}

void Text2D::EndPlacing() {
	auto&& layout = std::make_unique<VertexBufferLayout>();
	layout->Push<float>(2);
	layout->Push<float>(2);
	auto& factory = RendererAPI::factory();
	auto&& vbo    = factory.NewVertexBuffer(
      characters_vertices_.data(),
      characters_vertices_.size() * sizeof(CharacterVertexBuffer),
      std::move(layout));
	auto&& ibo = factory.NewIndexBuffer(&character_indices_.data()->x,
	                                    character_indices_.size() * 3);
	vao_       = factory.NewVertexArray(std::move(vbo), std::move(ibo));
}

void Text2D::Reinit() {
	EndPlacing();
}

}  // namespace renderer
