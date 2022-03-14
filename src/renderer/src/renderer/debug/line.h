#pragma once

#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/quaternion_float.hpp>
#include <glm/mat2x3.hpp>

#include "renderer/renderer_api.h"
#include "renderer/scene_object.h"

namespace debug {

class Line : public SceneObject {
 public:
	explicit Line(glm::mat2x3 location);
	void Render(const glm::mat4& parentTransform) {  }
 private:
	std::unique_ptr<VertexArray> vao_;
};

} // namespace debug
