#include "vertexbuffer.h"

#include <glad/glad.h>

#include <memory>

#include "renderer/vertexbufferlayout.h"

OpenGLVertexBuffer::OpenGLVertexBuffer(size_t size) {
	glGenBuffers(1, &id_);
	Bind();
	glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
}

OpenGLVertexBuffer::OpenGLVertexBuffer(
		const void* data, size_t size,
		std::unique_ptr<VertexBufferLayout>&& layout) {
	glGenBuffers(1, &id_);
	Bind();
	glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
	layout_ = std::move(layout);
}

OpenGLVertexBuffer::~OpenGLVertexBuffer() { glDeleteBuffers(1, &id_); }

OpenGLVertexBuffer::OpenGLVertexBuffer(OpenGLVertexBuffer&& other) noexcept {
	if (this == &other) return;
	*this = std::move(other);
}

OpenGLVertexBuffer& OpenGLVertexBuffer::operator=(
		OpenGLVertexBuffer&& other) noexcept {
	if (this == &other) return *this;
	// Delete owned buffer
	glDeleteBuffers(1, &id_);
	// Assign moving buffer data
	id_ = other.id_;
	layout_ = std::move(other.layout_);
	// Invalidate moving buffer
	other.id_ = 0;

	return *this;
}

void OpenGLVertexBuffer::Bind() const { glBindBuffer(GL_ARRAY_BUFFER, id_); }

void OpenGLVertexBuffer::Unbind() const { glBindBuffer(GL_ARRAY_BUFFER, 0); }

void OpenGLVertexBuffer::SetData(const void* data, size_t size,
                                 std::unique_ptr<VertexBufferLayout>&& layout) {
	Bind();
	glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);

	layout_ = std::move(layout);
}
