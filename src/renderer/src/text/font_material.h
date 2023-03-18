#pragma once

#include <glm/glm.hpp>

#include <memory>

#include "renderer/shaderprogram.h"
#include "renderer/texture2d.h"


namespace renderer {

class FontMaterial {
 public:
	FontMaterial(std::shared_ptr<Texture2D> font_atlas);

	void Use(const glm::mat4& view) {
		atlas_->Bind(0);
		shader_->Use();
		shader_->SetMat4(view_shader_var_, view);
	}

	void Unuse() {
		shader_->Unuse();
		atlas_->Unbind(0);
	}

 private:
	std::shared_ptr<Texture2D> atlas_;
	std::shared_ptr<ShaderProgram> shader_;

	const char* view_shader_var_ = "u_View";
};

}  // namespace renderer
