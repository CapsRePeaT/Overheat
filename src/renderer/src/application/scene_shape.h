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
	[[nodiscard]] GlobalId id() const { return core_shape_.id(); }
	[[nodiscard]] LayerId layer_id() const { return core_shape_.layer_id(); }
	inline void SetIsVisible(bool is_visible) { is_visible_ = is_visible; }
	[[nodiscard]] inline bool is_visible() const { return is_visible_; }
	

 private:
	const BasicShape& core_shape_;
	std::unique_ptr<VertexArray> vao_;
	bool is_visible_ = true;
};

}  // namespace renderer
