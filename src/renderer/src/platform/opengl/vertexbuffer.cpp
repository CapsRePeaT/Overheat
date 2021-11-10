#include "vertexbuffer.h"

#include <glad/glad.h>

GLuint OpenGLVertexBuffer::bound_id_;

OpenGLVertexBuffer::OpenGLVertexBuffer(size_t size) {
	glGenBuffers(1, &id_);
	Bind();
	glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
}

OpenGLVertexBuffer::OpenGLVertexBuffer(size_t size, const void* data) {
	glGenBuffers(1, &id_);
	Bind();
	glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

OpenGLVertexBuffer::~OpenGLVertexBuffer() {
	if (id_ == bound_id_) bound_id_ = 0;
	glDeleteBuffers(1, &id_);
}

OpenGLVertexBuffer::OpenGLVertexBuffer(OpenGLVertexBuffer&& other) noexcept {
	*this = std::move(other);
}

OpenGLVertexBuffer& OpenGLVertexBuffer::operator=(
		OpenGLVertexBuffer&& other) noexcept {
	// Delete owned buffer
	glDeleteBuffers(1, &id_);
	// Assign moving buffer data
	id_ = other.id_;
	// Invalidate moving buffer
	other.id_ = 0;

	return *this;
}

void OpenGLVertexBuffer::Bind() const {
	if (id_ != bound_id_) {
		glBindBuffer(GL_ARRAY_BUFFER, id_);
		bound_id_ = id_;
	}
}

void OpenGLVertexBuffer::Unbind() const {
	if (bound_id_ != 0) {
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		bound_id_ = 0;
	}
}

void OpenGLVertexBuffer::SetData(size_t size, const void* data) {
	Bind();
	glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);
}
