#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <memory>

#include "renderer/scene_object.h"
#include "shapes.h"
#include "renderer/vertexarray.h"

class SceneShape : public SceneObject {
 public:
	explicit SceneShape(const BasicShape& shape);
	[[nodiscard]] const VertexArray& vertex_array() const { return *vao_; }

 private:
	const size_t id_;
	std::unique_ptr<VertexArray> vao_;
};
