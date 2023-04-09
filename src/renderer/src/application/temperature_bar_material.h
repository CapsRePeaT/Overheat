#pragma once

#include <array>
#include <memory>

#include "renderer/shaderprogram.h"

namespace renderer {

class TemperatureBarMaterial {
 public:
	TemperatureBarMaterial();

	void SetColors(const std::array<glm::vec3, 5>& colors) { colors_ = colors; }

	void Use(const glm::mat4& view) {
		shader_->Use();
		shader_->SetVec3v(color_shader_var_, colors_.data(), colors_.size());
		shader_->SetMat4(view_shader_var_, view);
	}

	void Unuse() { shader_->Unuse(); }

 private:
	std::shared_ptr<ShaderProgram> shader_;
	std::array<glm::vec3, 5> colors_ =
			std::to_array<glm::vec3, 5>({{0.0f, 0.0f, 1.0f},
	                                 {0.0f, 1.0f, 1.0f},
	                                 {0.0f, 1.0f, 0.0f},
	                                 {1.0f, 1.0f, 0.0f},
	                                 {1.0f, 0.0f, 0.0f}});

	const char* view_shader_var_  = "u_View";
	const char* color_shader_var_ = "u_Colors";
};

}  // namespace renderer
