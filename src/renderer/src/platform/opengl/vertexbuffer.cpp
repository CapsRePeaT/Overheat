#include "vertexbuffer.h"

#include <glad/glad.h>
#include <spdlog/spdlog.h>

#include <memory>

#include "constants.h"
#include "renderer/vertexbufferlayout.h"
#include "log.h"

namespace renderer::gl {

namespace {

uint32_t InitBuffer(const size_t size, const void* data, const int draw_type) {
	if (!data && draw_type == GL_STATIC_DRAW)
		spdlog::warn("Creating empty static vertex buffer");
	uint32_t id;
	glGenBuffers(consts::buffer_count_one, &id);
	// LOG_TRACE("VB id={} created", id);
	glBindBuffer(GL_ARRAY_BUFFER, id);
	glBufferData(GL_ARRAY_BUFFER, size, data, draw_type);
	return id;
}

}  // namespace

VertexBuffer::VertexBuffer(const size_t size)
		: id_(InitBuffer(size, nullptr, GL_DYNAMIC_DRAW)) {}

VertexBuffer::VertexBuffer(const void* data, const size_t size,
                           std::unique_ptr<VertexBufferLayout>&& layout)
		: id_(InitBuffer(size, data, GL_STATIC_DRAW)), layout_(std::move(layout)) {}

VertexBuffer::~VertexBuffer() {
	// LOG_TRACE("VB id={} deleted", id_);
	glDeleteBuffers(consts::buffer_count_one, &id_);
}

VertexBuffer::VertexBuffer(VertexBuffer&& other) noexcept {
	*this = std::move(other);
}

VertexBuffer& VertexBuffer::operator=(VertexBuffer&& other) noexcept {
	if (this == &other)
		return *this;
	// Delete owned buffer
	// LOG_TRACE("VB id={} delete", id_);
	glDeleteBuffers(consts::buffer_count_one, &id_);
	// LOG_TRACE("VB id={} deleted", id_);
	// Assign moving buffer data
	id_     = other.id_;
	layout_ = std::move(other.layout_);
	// Invalidate moving buffer
	other.id_ = 0;

	return *this;
}

void VertexBuffer::Bind() const { 
	// LOG_TRACE("VB id={} binded", id_);
	glBindBuffer(GL_ARRAY_BUFFER, id_); 
}

void VertexBuffer::Unbind() const {
	// LOG_TRACE("VB id={} unbinded", id_);
	glBindBuffer(GL_ARRAY_BUFFER, 0); 
}

void VertexBuffer::SetData(const void* data, const size_t size,
                           std::unique_ptr<VertexBufferLayout>&& layout) {
	Bind();
	glBufferSubData(GL_ARRAY_BUFFER, /*offset=*/0, size, data);
	layout_ = std::move(layout);
	Unbind();
}

}  // namespace renderer::gl
