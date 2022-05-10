#pragma once

#include <array>
#include <memory>

#include "renderer/scene_object.h"
#include "renderer/vertexarray.h"

namespace renderer::debug {

class Axes : public SceneObject {
 public:
	explicit Axes();
	[[nodiscard]] const VertexArray& vertex_array() const { return *vao_; }

 private:
	std::unique_ptr<VertexArray> vao_;
};

}  // namespace renderer::debug
