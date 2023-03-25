#pragma once

#include <glm/glm.hpp>

#include <array>
#include <memory>
#include <vector>
#include <string>

#include "renderer/vertexarray.h"


namespace renderer {

struct CharacterVertex {
	glm::vec2 pos;
	glm::vec2 uv;
};

struct CharacterVertexBuffer {
	std::array<CharacterVertex, 4> vertices;
};

static_assert(sizeof(CharacterVertex) == sizeof(glm::vec2) * 2);
static_assert(sizeof(CharacterVertexBuffer) == sizeof(CharacterVertex) * 4);

class Text2D {
 public:
	void BeginPlacing(std::u32string text, glm::vec2 position,
                                         const bool align_to_pixels);
	void AddChar(CharacterVertex top_left, CharacterVertex bottom_right);
	void EndPlacing();

	void Reinit();

	[[nodiscard]] const VertexArray& vertex_array() const { return *vao_; }
	[[nodiscard]] const std::u32string& text() const { return text_; }
	[[nodiscard]] glm::vec2 position() const { return position_ ; }
	[[nodiscard]] bool align_to_pixels() const { return align_to_pixels_; }

 private:
	static constexpr glm::u32vec3 character_index_buffer_base1 = {0u, 1u, 3u};
	static constexpr glm::u32vec3 character_index_buffer_base2 = {1u, 2u, 3u};
	std::vector<CharacterVertexBuffer> characters_vertices_;
	std::vector<glm::u32vec3> character_indices_;
	std::unique_ptr<VertexArray> vao_;
	std::u32string text_;
	glm::vec2 position_;
	bool align_to_pixels_;
};

}  // namespace renderer
