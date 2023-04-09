#pragma once

#include <memory>

#include "renderer/shaderprogram.h"

namespace renderer {

class TemperatureBarMaterial {
 public:
	TemperatureBarMaterial();

	void SetColors(glm::vec3 min, glm::vec3 max) {
		color_range_[0] = min;
		color_range_[1] = max;
	}

	void Use(const glm::mat4& view) {
		shader_->Use();
		shader_->SetMat2x3(color_range_shader_var_, color_range_);
		shader_->SetMat4(view_shader_var_, view);
	}

	void Unuse() { shader_->Unuse(); }

 private:
	std::shared_ptr<ShaderProgram> shader_;
	glm::mat2x3 color_range_ = {{0.0f, 0.0f, 1.0f}, {1.0f, 0.0f, 0.0f}};

	const char* view_shader_var_        = "u_View";
	const char* color_range_shader_var_ = "u_ColorRange";
};

}  // namespace renderer
