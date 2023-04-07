#pragma once

#include <memory>

#include "common.h"
#include "heatmap_material.h"
#include "renderer/drawable.h"
#include "renderer/scene_object.h"
#include "renderer/vertexarray.h"
#include "shapes.h"

namespace renderer {

class BoxShape : public SceneObject, public Drawable {
 public:
	explicit BoxShape(const BasicShape& shape);
	[[nodiscard]] inline const BasicShape& core_shape() const {
		return core_shape_;
	}
	[[nodiscard]] const VertexArray& vertex_array() const { return *vao_; }
	[[nodiscard]] GlobalId id() const { return core_shape_.id(); }
	[[nodiscard]] LayerId layer_id() const { return core_shape_.layer_id(); }
	inline void SetIsVisible(bool is_visible) { is_visible_ = is_visible; }
	[[nodiscard]] inline bool is_visible() const { return is_visible_; }
	inline void SetHighlightType(HighlightType highlight_type) {
		highlight_type_ = highlight_type;
	}
	[[nodiscard]] inline HighlightType highlight_type() const {
		return highlight_type_;
	}
	bool SetContextForDraw(RendererContext& ctx) override;
	// Must be called only from scene
	void SetMaterial(HeatmapMaterial& material) { material_ = &material; }

 private:
	const BasicShape& core_shape_;
	std::unique_ptr<VertexArray> vao_;
	HighlightType highlight_type_ = HighlightType::None;
	bool is_visible_              = true;
	HeatmapMaterial* material_;
};

}  // namespace renderer