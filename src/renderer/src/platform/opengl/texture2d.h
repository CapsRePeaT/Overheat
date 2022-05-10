#pragma once

#include <glad/glad.h>

#include <string>

#include "renderer/texture2d.h"

namespace renderer::gl {

class Texture2D : public renderer::Texture2D {
 public:
	Texture2D(int width, int height, const void* data, int channels = 4,
	                int32_t filter       = GL_LINEAR,
	                int32_t wrap_mode    = GL_CLAMP_TO_EDGE,
	                bool generate_bitmap = false);

	Texture2D(Texture2D&& other) noexcept;
	Texture2D& operator=(Texture2D&& other) noexcept;
	~Texture2D() override;
	Texture2D()                       = delete;
	Texture2D(const Texture2D&) = delete;
	Texture2D& operator=(const Texture2D&) = delete;

	void Bind(int unit = -1) const override;

 private:
	uint32_t id_;
	int32_t mode_;
	int32_t width_;
	int32_t height_;

	inline static uint32_t bound_id = 0;
	inline static int active_unit   = 0;
};

}  // namespace renderer::gl
