#pragma once

#include <memory>

#include "common.h"
#include "renderer/scene_object.h"
#include "renderer/vertexarray.h"
#include "shapes.h"

namespace renderer {

class SceneShape : public SceneObject {
 public:
	explicit SceneShape(const BasicShape& shape);
	[[nodiscard]] inline const BasicShape& core_shape() const {
		return core_shape_;
	}
	[[nodiscard]] const VertexArray& vertex_array() const { return *vao_; }

 private:
	const BasicShape& core_shape_;
	std::unique_ptr<VertexArray> vao_;
};

}  // namespace renderer
