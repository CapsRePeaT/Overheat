#include "texture2d.h"

#include <cstddef>

namespace renderer::gl {

// TODO: get data type as template parameter and set type for glTextImage2D by it
Texture2D::Texture2D(const GLsizei width, GLsizei height,
                                 const void* data, const int channels,
                                 const int32_t filter, const int32_t wrap_mode,
                                 bool generate_bitmap)
		: id_(0), width_(width), height_(height) {
	switch (channels) {
		case 1:
			mode_ = GL_RED;
			break;
		case 3:
			mode_ = GL_RGB;
			break;
		case 4:
			mode_ = GL_RGBA;
			break;
		default:
			mode_ = GL_RGBA;
			break;
	}

	glGenTextures(1, &id_);

	glBindTexture(GL_TEXTURE_2D, id_);
	glTexImage2D(GL_TEXTURE_2D, 0, mode_, width_, height_, 0, mode_,
	             GL_FLOAT, data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrap_mode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrap_mode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filter);

	if (generate_bitmap)
		glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);
}

Texture2D::Texture2D(Texture2D&& other) noexcept
		: id_(other.id_),
			mode_(other.mode_),
			width_(other.width_),
			height_(other.height_) {
	if (this == &other)
		return;
	*this = std::move(other);
}

Texture2D& Texture2D::operator=(Texture2D&& other) noexcept {
	if (this == &other)
		return *this;
	id_       = other.id_;
	other.id_ = 0;

	mode_ = other.mode_;

	width_  = other.width_;
	height_ = other.height_;

	return *this;
}

Texture2D::~Texture2D() {
	if (bound_id == id_)
		bound_id = 0;
	glDeleteTextures(1, &id_);
}

int32_t GetMaxTextureImageUnits() {
	int32_t texture_unit_amount = 0;
	glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &texture_unit_amount);
	return texture_unit_amount;
}

void Texture2D::Bind(const int unit) const {
	if (unit != active_unit && unit >= 0) {
		thread_local int32_t texture_unit_amount = GetMaxTextureImageUnits();
		if (unit < texture_unit_amount) {
			GLenum active_texture = GL_TEXTURE0 + unit;
			glActiveTexture(active_texture);
			active_unit = unit;
		}
	}
	if (id_ != bound_id) {
		glBindTexture(GL_TEXTURE_2D, id_);
		bound_id = id_;
	}
}

}  // namespace renderer::gl
