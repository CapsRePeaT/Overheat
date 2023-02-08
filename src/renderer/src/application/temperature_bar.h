#pragma once

#include <memory>
#include <vector>

#include "glm/glm.hpp"

#include "renderer/vertexarray.h"

namespace renderer {

class TemperatureBar {
 public:
	TemperatureBar(float width, float height, glm::vec2 location);
	[[nodiscard]] const VertexArray& vertex_array() const { return *vao_; }

 private:
	std::unique_ptr<VertexArray> vao_;
};

}  // namespace renderer