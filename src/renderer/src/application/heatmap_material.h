#pragma once

#include <glm/ext/vector_float2.hpp>
#include <glm/fwd.hpp>
#include <glm/glm.hpp>

#include <array>
#include <cassert>
#include <memory>

#include "constants.h"
#include "renderer/shaderprogram.h"
#include "renderer/texture2d.h"

namespace renderer {

class HeatmapMaterial {
 public:
	HeatmapMaterial(
			std::pair<std::unique_ptr<Texture2D>, std::unique_ptr<Texture2D>>
					heatmap_textures,
			std::pair<glm::vec2, glm::vec2> bot_top_temp_ranges,
			std::pair<float, float> bounds);

	void SetTemperatureRange(float min, float max) {
		temperature_range_[0] = min;
		temperature_range_[1] = max;
	}
	void SetColors(const std::array<glm::vec3, 5>& colors) { colors_ = colors; }
	void SetIsStratified(bool value) { is_stratified_ = value; }
	void SetStratifiedStep(float step) { stratified_step_ = step; }
	void SetShader(std::shared_ptr<ShaderProgram> shader) {
		// Taking by-value and std::move to shader_ for explicity for
		// client code, that there will be ref count incrementing and to allow for
		// std::move for avoiding that
		shader_ = std::move(shader);
	}
	void Use(const glm::mat4& transform, const glm::mat4& view_projection,
	         const glm::vec3 edge_color, const bool is_transparent_heatmap) {
		assert(bot_heatmap_texture_ != nullptr);

		bot_heatmap_texture_->Bind(0);
		top_heatmap_texture_->Bind(1);
		shader_->Use();
		shader_->SetVec2(temperature_range_shader_var_, temperature_range_);
		shader_->SetVec3v(color_shader_var_, colors_.data(), colors_.size());
		shader_->SetMat4(transform_shader_var_, transform);
		shader_->SetMat4(view_proj_shader_var_, view_projection);
		shader_->SetVec2(bottom_temp_range_var_, bot_temp_range_);
		shader_->SetVec2(top_temp_range_var_, top_temp_range_);
		shader_->SetMat2(bounds_var_, bounds_);
		shader_->SetBool(is_stratified_var, is_stratified_);
		shader_->SetFloat(stratified_step_var, stratified_step_);
		shader_->SetVec3(edge_color_var, edge_color);
		shader_->SetBool(is_transparent_heatmap_var, is_transparent_heatmap);
	}

	void Unuse() {
		shader_->Unuse();
		bot_heatmap_texture_->Unbind(0);
		top_heatmap_texture_->Unbind(1);
	}

 private:
	std::shared_ptr<ShaderProgram> shader_;
	glm::vec2 temperature_range_ = {0.0f, 100.0f};
	std::array<glm::vec3, 5> colors_ =
			std::to_array<glm::vec3, 5>({{0.0f, 0.0f, 1.0f},
	                                 {0.0f, 1.0f, 1.0f},
	                                 {0.0f, 1.0f, 0.0f},
	                                 {1.0f, 1.0f, 0.0f},
	                                 {1.0f, 0.0f, 0.0f}});
	glm::vec2 bot_temp_range_ = temperature_range_;
	glm::vec2 top_temp_range_ = temperature_range_;
	std::unique_ptr<Texture2D> bot_heatmap_texture_;
	std::unique_ptr<Texture2D> top_heatmap_texture_;
	glm::mat2 bounds_;
	float stratified_step_ = consts::init::default_stratified_step;
	bool is_stratified_    = false;

	// TODO: move to templated base class
	const char* view_proj_shader_var_         = "u_ViewProjection";
	const char* transform_shader_var_         = "u_ModelTransform";
	const char* temperature_range_shader_var_ = "u_TemperatureRange";
	const char* color_shader_var_             = "u_Colors";
	const char* bottom_temp_range_var_        = "u_BottomTemperatureRange";
	const char* top_temp_range_var_           = "u_TopTemperatureRange";
	const char* bottom_heatmap_var_           = "u_BottomHeatmap";
	const char* top_heatmap_var_              = "u_TopHeatmap";
	const char* bounds_var_                   = "u_Bounds";
	const char* is_stratified_var             = "u_IsStratified";
	const char* stratified_step_var           = "u_StratifiedStep";
	const char* edge_color_var                = "u_edgeColor";
	const char* is_transparent_heatmap_var    = "u_IsTransparentHeatmap";
};
}  // namespace renderer
