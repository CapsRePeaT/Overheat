#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <memory>

#include "shapes.h"
#include "renderer/vertexarray.h"

class SceneShape {
 public:
	SceneShape(const BasicShape& shape);
	void Rotate(float angle, glm::vec3 axis) {
		rotation_ = glm::rotate(rotation_, angle, axis);
	}
	[[nodiscard]] glm::mat4 transform() const {
		const glm::mat4 rotation = glm::mat4_cast(rotation_);
		return glm::translate(glm::mat4(1.0f), position_) * rotation *
		       glm::scale(glm::mat4(1.0f), scale_);
	}
	[[nodiscard]] const VertexArray& vertex_array() const { return *vao_; }

 private:
	const size_t id_;
	std::unique_ptr<VertexArray> vao_;
	glm::vec3 position_ = {0.0f, 0.0f, 0.0f};
	glm::vec3 scale_ = {1.0f, 1.0f, 1.0f};
	glm::quat rotation_ = glm::quat({0.0f, 0.0f, 0.0f});
};
