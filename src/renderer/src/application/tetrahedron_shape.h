#pragma once

#include "../../fem_solver/src/solver_shapes.hpp"
#include "box_shape.h"
#include "common.h"
#include "renderer/drawable.h"
#include "renderer/scene_object.h"
#include "renderer/vertexarray.h"
#include "tetrahedron_material.h"


namespace renderer {

class TetrahedronShape : public SceneObject, public Drawable {
 public:
	TetrahedronShape(
			const std::vector<std::shared_ptr<SolverTetraeder>>& core_shapes,
			std::shared_ptr<VertexBuffer> points, std::shared_ptr<VertexBuffer> temps,
			TetrahedronMaterial& material, const std::vector<glm::vec3>& coords);
	[[nodiscard]] const VertexArray& vertex_array() const { return *vao_; }
	inline void SetHighlightType(HighlightType highlight_type) {
		shape_for_wireframe_->SetHighlightType(highlight_type);
	}
	[[nodiscard]] inline HighlightType highlight_type() const {
		return shape_for_wireframe_->highlight_type();
	}
	bool SetContextForDraw(RendererContext& ctx) override;
	bool SetContextForDrawWireframe(RendererContext& ctx) override;

 private:
	std::unique_ptr<VertexArray> vao_;
	TetrahedronMaterial* material_;
	// TODO: add new class for wireframes and remove box_shape from here
	std::unique_ptr<BoxShape> shape_for_wireframe_;
};

}  // namespace renderer
