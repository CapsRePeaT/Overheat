#pragma once

#include <glm/glm.hpp>
#include <memory>

#include "i_renderer.h"
#include "renderer/shaderprogram.h"

class HeatmapMaterial {
 public:
	HeatmapMaterial();
	void SetTemperatureRange(float min, float max) {
		temperature_range_[0] = min;
		temperature_range_[1] = max;
	}
	void SetColorRange(const glm::vec3& min, const glm::vec3& max) {
		color_range_[0] = min;
		color_range_[1] = max;
	}
	void SetShader(const std::shared_ptr<ShaderProgram>& shader) {
		shader_ = shader;
	}

	void Use(const glm::mat4& transform, const glm::mat4 view_projection) {
		shader_->Use();
		shader_->SetVec2(temperature_range_shader_var_, temperature_range_);
		shader_->SetMat2x3(color_range_shader_var_, color_range_);
		shader_->SetMat4(transform_shader_var_, transform);
		shader_->SetMat4(view_proj_shader_var_, view_projection);
	}

 private:
	std::shared_ptr<ShaderProgram> shader_;
	glm::vec2 temperature_range_ = {300.0f, 400.0f};
	glm::mat2x3 color_range_ = {{0.0f, 0.0f, 1.0f}, {1.0f, 0.0f, 0.0f}};

	// TODO: move to templated base class
	const char* temperature_range_shader_var_ = "u_TemperatureRange";
	const char* color_range_shader_var_ = "u_ColorRange";
	const char* view_proj_shader_var_ = "u_ViewProjection";
	const char* transform_shader_var_ = "u_ModelTransform";
};
