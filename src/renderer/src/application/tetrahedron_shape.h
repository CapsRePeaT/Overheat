#pragma once

#include "../../fem_solver/src/solver_shapes.hpp"
#include "common.h"
#include "renderer/drawable.h"
#include "renderer/scene_object.h"
#include "renderer/vertexarray.h"
#include "tetrahedron_material.h"

namespace renderer {

class TetrahedronShape : public SceneObject, public Drawable {
 public:
	TetrahedronShape(std::shared_ptr<SolverTetraeder> core_shape,
	                 std::shared_ptr<VertexBuffer> points,
	                 std::shared_ptr<VertexBuffer> temps,
									 TetrahedronMaterial& material);

	[[nodiscard]] const VertexArray& vertex_array() const { return *vao_; }
	bool SetContextForDraw(RendererContext& ctx);

 private:
 	const std::shared_ptr<SolverTetraeder> core_shape_;
	std::unique_ptr<VertexArray> vao_;
	TetrahedronMaterial* material_;
};

}  // namespace renderer
