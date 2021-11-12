#include "indexbuffer.h"

#include <glad/glad.h>

OpenGLIndexBuffer::OpenGLIndexBuffer(const uint32_t* data, size_t count)
		: id_(0), count_(count) {
	glGenBuffers(1, &id_);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), data,
	             GL_STATIC_DRAW);
}

OpenGLIndexBuffer::~OpenGLIndexBuffer() { glDeleteBuffers(1, &id_); }

OpenGLIndexBuffer::OpenGLIndexBuffer(OpenGLIndexBuffer&& other) noexcept {
	if (this == &other) return;
	*this = std::move(other);
}

OpenGLIndexBuffer& OpenGLIndexBuffer::operator=(
		OpenGLIndexBuffer&& other) noexcept {
	if (this == &other) return *this;
	// Delete owned buffer
	glDeleteBuffers(1, &id_);
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
