#include "indexbuffer.h"

#include <glad/glad.h>

#include "constants.h"

OpenGLIndexBuffer::OpenGLIndexBuffer(const uint32_t* data, const size_t count)
		: count_(count) {
	glGenBuffers(consts::buffer_count_one, &id_);
	Bind();
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), data,
	             GL_STATIC_DRAW);
}

OpenGLIndexBuffer::~OpenGLIndexBuffer() { glDeleteBuffers(consts::buffer_count_one, &id_); }

OpenGLIndexBuffer::OpenGLIndexBuffer(OpenGLIndexBuffer&& other) noexcept {
	*this = std::move(other);
}

OpenGLIndexBuffer& OpenGLIndexBuffer::operator=(
		OpenGLIndexBuffer&& other) noexcept {
	if (this == &other) return *this;
	// Delete owned buffer
	glDeleteBuffers(consts::buffer_count_one, &id_);
	// Assign moving buffer data
	id_ = other.id_;
	count_ = other.count_;
	// Invalidate moving buffer
	other.id_ = 0;
	other.count_ = 0;
	return *this;
}

void OpenGLIndexBuffer::Bind() const {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_);
}

void OpenGLIndexBuffer::Unbind() const {
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
