#include "vertexbuffer.h"

GLuint VertexBuffer::bound_id_;

VertexBuffer::VertexBuffer(const void* data, size_t size,
                           BufferType buffer_type) {
	glGenBuffers(1, &id_);
	Bind();
	glBufferData(GL_ARRAY_BUFFER, size, data, static_cast<GLenum>(buffer_type));
}

VertexBuffer::~VertexBuffer() {
	if (id_ == bound_id_) bound_id_ = 0;
	glDeleteBuffers(1, &id_);
}

VertexBuffer::VertexBuffer(VertexBuffer&& other) noexcept {
	*this = std::move(other);
}

VertexBuffer& VertexBuffer::operator=(VertexBuffer&& other) noexcept {
	// Delete owned buffer
	glDeleteBuffers(1, &id_);
	// Assign moving buffer data
	id_ = other.id_;
	// Invalidate moving buffer
	other.id_ = 0;

	return *this;
}

void VertexBuffer::Bind() const {
	if (id_ != bound_id_) {
		glBindBuffer(GL_ARRAY_BUFFER, id_);
		bound_id_ = id_;
	}
}

void VertexBuffer::Unbind() {
	if (bound_id_ != 0) {
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		bound_id_ = 0;
	}
}

void VertexBuffer::BufferSubData(GLsizeiptr size, const void* data,
                                 GLintptr offset) {
	Bind();
	glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
}
