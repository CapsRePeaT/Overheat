#include "indexbuffer.h"

IndexBuffer::IndexBuffer(const GLuint* data, size_t count)
		: id_(0), count_(count) {
	glGenBuffers(1, &id_);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(GLuint), data,
	             GL_STATIC_DRAW);
}

IndexBuffer::~IndexBuffer() { glDeleteBuffers(1, &id_); }

IndexBuffer::IndexBuffer(IndexBuffer&& other) noexcept {
	*this = std::move(other);
}

IndexBuffer& IndexBuffer::operator=(IndexBuffer&& other) noexcept {
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

void IndexBuffer::Bind() const { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id_); }

void IndexBuffer::Unbind() { glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); }
