#pragma once

#include <glm/ext/vector_float2.hpp>
#include <glm/fwd.hpp>
#include <glm/glm.hpp>

#include <cassert>
#include <memory>

#include "renderer/shaderprogram.h"
#include "renderer/texture2d.h"

namespace renderer {

class HeatmapMaterial {
 public:
	HeatmapMaterial(
			std::pair<std::unique_ptr<Texture2D>, std::unique_ptr<Texture2D>>
					heatmap_textures,
			std::pair<glm::vec2, glm::vec2> bot_top_temp_ranges);

	void SetTemperatureRange(float min, float max) {
		temperature_range_[0] = min;
		temperature_range_[1] = max;
	}
	void SetColorRange(glm::vec3 min, glm::vec3 max) {
		color_range_[0] = min;
		color_range_[1] = max;
	}
	void SetShader(std::shared_ptr<ShaderProgram> shader) {
		// Taking by-value and std::move to shader_ for explicity for
		// client code, that there will be ref count incrementing and to allow for
		// std::move for avoiding that
		shader_ = std::move(shader);
	}
	void Use(const glm::mat4& transform, const glm::mat4& view_projection,
	         glm::mat2 bounds) {
		assert(bot_heatmap_texture_ != nullptr);

		bot_heatmap_texture_->Bind(0);
		top_heatmap_texture_->Bind(1);
		shader_->Use();
		shader_->SetVec2(temperature_range_shader_var_, temperature_range_);
		shader_->SetMat2x3(color_range_shader_var_, color_range_);
		shader_->SetMat4(transform_shader_var_, transform);
		shader_->SetMat4(view_proj_shader_var_, view_projection);
		shader_->SetVec2(bottom_temp_range_var_, bot_temp_range_);
		shader_->SetVec2(top_temp_range_var_, top_temp_range_);
		shader_->SetMat2(bounds_var_, bounds); 
	}

 private:
	std::shared_ptr<ShaderProgram> shader_;
	glm::vec2 temperature_range_ = {0.0f, 100.0f};
	glm::mat2x3 color_range_     = {{0.0f, 0.0f, 1.0f}, {1.0f, 0.0f, 0.0f}};
	glm::vec2 bot_temp_range_    = temperature_range_;
	glm::vec2 top_temp_range_    = temperature_range_;
	std::unique_ptr<Texture2D> bot_heatmap_texture_;
	std::unique_ptr<Texture2D> top_heatmap_texture_;

	// TODO: move to templated base class
	const char* view_proj_shader_var_         = "u_ViewProjection";
	const char* transform_shader_var_         = "u_ModelTransform";
	const char* temperature_range_shader_var_ = "u_TemperatureRange";
	const char* color_range_shader_var_       = "u_ColorRange";
	const char* bottom_temp_range_var_        = "u_BottomTemperatureRange";
	const char* top_temp_range_var_           = "u_TopTemperatureRange";
	const char* bottom_heatmap_var_           = "u_BottomHeatmap";
	const char* top_heatmap_var_              = "u_TopHeatmap";
	const char* bounds_var_                   = "u_Bounds";
};

}  // namespace renderer
