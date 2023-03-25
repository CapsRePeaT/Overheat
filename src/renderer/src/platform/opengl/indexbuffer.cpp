#include "indexbuffer.h"

#include <glad/glad.h>

#include "constants.h"
#include "log.h"

namespace renderer::gl {

IndexBuffer::IndexBuffer(const uint32_t* data, const size_t count)
		: count_(count) {
	glGenBuffers(consts::buffer_count_one, &id_);
	// LOG_TRACE("ib id={} created", id_);
	Bind();
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), data,
	             GL_STATIC_DRAW);
	Unbind();
}

IndexBuffer::~IndexBuffer() {
	// LOG_TRACE("ib id={} deleted", id_);
	glDeleteBuffers(consts::buffer_count_one, &id_);
}

IndexBuffer::IndexBuffer(IndexBuffer&& other) noexcept {
	*this = std::move(other);
}

IndexBuffer& IndexBuffer::operator=(IndexBuffer&& other) noexcept {
	if (this == &other)
		return *this;
	// Delete owned buffer
	// LOG_TRACE("ib id={} delete", id_);
	glDeleteBuffers(consts::buffer_count_one, &id_);
	// LOG_TRACE("ib id={} deleted", id_);
	// Assign moving buffer data
	id_    = other.id_;
	count_ = other.count_;
	// Invalidate moving buffer
	other.id_    = 0;
	other.count_ = 0;
	return *this;
}

void IndexBuffer::Bind() const { 
	// LOG_TRACE("ib id={} binded", id_);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_); 
}

void IndexBuffer::Unbind() const { 
	// LOG_TRACE("ib id={} unbinded", id_);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); 
}

}  // namespace renderer::gl
