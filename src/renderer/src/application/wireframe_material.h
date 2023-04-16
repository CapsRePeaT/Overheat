#pragma once

#include <glm/ext/vector_float2.hpp>
#include <glm/fwd.hpp>
#include <glm/glm.hpp>

#include <array>
#include <cassert>
#include <memory>

#include "constants.h"
#include "renderer/shaderprogram.h"

namespace renderer {

class WireframeMaterial {
 public:
	WireframeMaterial();

	void Use(const glm::mat4& transform, const glm::mat4& view_projection,
	         const glm::vec3 edge_color) {
		shader_->Use();
		shader_->SetMat4(transform_shader_var_, transform);
		shader_->SetMat4(view_proj_shader_var_, view_projection);
		shader_->SetVec3(edge_color_var, edge_color);
	}

	void Unuse() { shader_->Unuse(); }

 private:
	std::shared_ptr<ShaderProgram> shader_;

	const char* view_proj_shader_var_ = "u_ViewProjection";
	const char* transform_shader_var_ = "u_ModelTransform";
	const char* edge_color_var        = "u_edgeColor";
};

}  // namespace renderer
