#pragma once

#include <glm/glm.hpp>

#include <memory>

#include "i_scene_viewport.h"
#include "renderer/shaderprogram.h"

namespace debug {

class DebugMaterial {
 public:
	DebugMaterial();
	void Use(const glm::mat4& transform, const glm::mat4& view_projection, float alpha) {
		shader_->Use();
		shader_->SetFloat(alpha_shader_var, alpha);
		shader_->SetMat4(transform_shader_var_, transform);
		shader_->SetMat4(view_proj_shader_var_, view_projection);
	}

 private:
	std::shared_ptr<ShaderProgram> shader_;
	// TODO: move to templated base class
	const char* alpha_shader_var      = "u_Alpha";
	const char* view_proj_shader_var_ = "u_ViewProjection";
	const char* transform_shader_var_ = "u_ModelTransform";
};

}  // namespace debug
