#pragma once

#include <array>
#include <memory>

#include "constants.h"
#include "glm/glm.hpp"
#include "renderer/shaderprogram.h"

namespace renderer {

class TetrahedronMaterial {
 public:
	TetrahedronMaterial();

	void SetTemperatureRange(float min, float max) {
		temperature_range_[0] = min;
		temperature_range_[1] = max;
	}
	void SetColors(const std::array<glm::vec3, 5>& colors) { colors_ = colors; }
	void SetIsStratified(bool value) { is_stratified_ = value; }
	void SetStratifiedStep(float step) { stratified_step_ = step; }

	void Use(const glm::mat4& transform, const glm::mat4& view_projection,
	         const bool is_transparent_heatmap) {
		shader_->Use();
		shader_->SetVec2(temperature_range_shader_var_, temperature_range_);
		shader_->SetVec3v(color_shader_var_, colors_.data(), colors_.size());
		shader_->SetMat4(transform_shader_var_, transform);
		shader_->SetMat4(view_proj_shader_var_, view_projection);
		shader_->SetBool(is_stratified_var, is_stratified_);
		shader_->SetFloat(stratified_step_var, stratified_step_);
		shader_->SetBool(is_transparent_heatmap_var, is_transparent_heatmap);
	}
	void Unuse() { shader_->Unuse(); }

 private:
	std::shared_ptr<ShaderProgram> shader_;
	glm::vec2 temperature_range_ = {0.0f, 100.0f};
	std::array<glm::vec3, 5> colors_ =
			std::to_array<glm::vec3, 5>({{0.0f, 0.0f, 1.0f},
	                                 {0.0f, 1.0f, 1.0f},
	                                 {0.0f, 1.0f, 0.0f},
	                                 {1.0f, 1.0f, 0.0f},
	                                 {1.0f, 0.0f, 0.0f}});
	float stratified_step_ = consts::init::default_stratified_step;
	bool is_stratified_    = false;

	const char* view_proj_shader_var_         = "u_ViewProjection";
	const char* transform_shader_var_         = "u_ModelTransform";
	const char* temperature_range_shader_var_ = "u_TemperatureRange";
	const char* color_shader_var_             = "u_Colors";
	const char* is_stratified_var             = "u_IsStratified";
	const char* stratified_step_var           = "u_StratifiedStep";
	const char* is_transparent_heatmap_var    = "u_IsTransparentHeatmap";
};

}  // namespace renderer
