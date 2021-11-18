#include "vertexbuffer.h"

#include <glad/glad.h>
#include <spdlog/spdlog.h>

#include <memory>

#include "constants.h"
#include "renderer/vertexbufferlayout.h"

uint32_t InitBuffer(const size_t size, const void* data, const int draw_type) {
	if (data == nullptr && draw_type == GL_STATIC_DRAW)
		spdlog::warn("Creating empty static vertex buffer");
	uint32_t id;
	glGenBuffers(consts::buffer_count_one, &id);
	glBindBuffer(GL_ARRAY_BUFFER, id);
	glBufferData(GL_ARRAY_BUFFER, size, data, draw_type);
	return id;
}

OpenGLVertexBuffer::OpenGLVertexBuffer(const size_t size)
		: id_(InitBuffer(size, nullptr, GL_DYNAMIC_DRAW)) {}

OpenGLVertexBuffer::OpenGLVertexBuffer(
		const void* data, size_t size, std::unique_ptr<VertexBufferLayout>&& layout)
		: id_(InitBuffer(size, data, GL_STATIC_DRAW)), layout_(std::move(layout)) {}

OpenGLVertexBuffer::~OpenGLVertexBuffer() {
	glDeleteBuffers(consts::buffer_count_one, &id_);
}

OpenGLVertexBuffer::OpenGLVertexBuffer(OpenGLVertexBuffer&& other) noexcept {
	if (this == &other) return;
	*this = std::move(other);
}

OpenGLVertexBuffer& OpenGLVertexBuffer::operator=(
		OpenGLVertexBuffer&& other) noexcept {
	if (this == &other) return *this;
	// Delete owned buffer
	glDeleteBuffers(consts::buffer_count_one, &id_);
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
	glBufferSubData(GL_ARRAY_BUFFER, /*offset=*/0, size, data);
	layout_ = std::move(layout);
}
