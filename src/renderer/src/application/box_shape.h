#pragma once

#include <memory>

#include "common.h"
#include "heatmap_material.h"
#include "renderer/drawable.h"
#include "renderer/scene_object.h"
#include "renderer/vertexarray.h"
#include "shapes.h"
#include "wireframe_material.h"

namespace renderer {

class BoxShape : public SceneObject, public Drawable {
 public:
	explicit BoxShape(const BasicShape& shape);
	[[nodiscard]] inline const BasicShape& core_shape() const {
		return core_shape_;
	}
	[[nodiscard]] const VertexArray& vertex_array() const { return *vao_; }
	[[nodiscard]] const VertexArray& wireframe_vao() const { return *wireframe_vao_; }
	[[nodiscard]] GlobalId id() const { return core_shape_.id(); }
	[[nodiscard]] LayerId layer_id() const { return core_shape_.layer_id(); }
	inline void SetHighlightType(HighlightType highlight_type) {
		highlight_type_ = highlight_type;
	}
	[[nodiscard]] inline HighlightType highlight_type() const {
		return highlight_type_;
	}
	bool SetContextForDraw(RendererContext& ctx) override;
	bool SetContextForDrawWireframe(RendererContext& ctx) override;
	// Must be called only from scene
	void SetMaterial(HeatmapMaterial& material) { material_ = &material; }

 private:
	const BasicShape& core_shape_;
	std::unique_ptr<VertexArray> vao_;
	HighlightType highlight_type_ = HighlightType::None;
	HeatmapMaterial* material_;
	std::unique_ptr<VertexArray> wireframe_vao_;
};

inline glm::vec3 HighlightTypeToColor(HighlightType highlight_type) {
	switch (highlight_type) {
		case HighlightType::None:
			return consts::color_not_selected;
		case HighlightType::Selected:
			return consts::color_selected;
		case HighlightType::ActiveSelected:
			return consts::color_active_selected;
		default:
			assert(false && "Unkown highlight type!");
			return consts::vec3_0;
	}
}


}  // namespace renderer
