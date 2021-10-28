#include <vertexbuffer.h>

#include <opengllog.h>

using namespace gl; // NOLINT(google-build-using-namespace)

GLuint VertexBuffer::boundId = 0;

VertexBuffer::VertexBuffer(const void* data, size_t size, BufferType bufferType) {
	glCall<glGenBuffers>(1, &m_id);
	bind();
	glCall<glBufferData>(GL_ARRAY_BUFFER, size, data, static_cast<GLenum>(bufferType));
}

VertexBuffer::~VertexBuffer() {
	if (m_id == boundId)
		boundId = 0;
	glDeleteBuffers(1, &m_id);
}

VertexBuffer::VertexBuffer(VertexBuffer&& other) noexcept {
	*this = std::move(other);
}

VertexBuffer& VertexBuffer::operator=(VertexBuffer&& other) noexcept {
	glDeleteBuffers(1, &m_id);

	m_id = other.m_id;

	other.m_id = 0;

	return *this;
}

void VertexBuffer::bind() const {
	if (m_id != boundId) {
		glCall<glBindBuffer>(GL_ARRAY_BUFFER, m_id);
		boundId = m_id;
	}
}

void VertexBuffer::unbind() {
	if (boundId != 0) {
		glCall<glBindBuffer>(GL_ARRAY_BUFFER, 0);
		boundId = 0;
	}
}

void VertexBuffer::bufferSubData(gl::GLsizeiptr size, const void* data, gl::GLintptr offset) {
	bind();
	glCall<glBufferSubData>(GL_ARRAY_BUFFER, offset, size, data);
}

