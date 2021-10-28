#include <indexbuffer.h>

#include <opengllog.h>

using namespace gl; // NOLINT(google-build-using-namespace)

IndexBuffer::IndexBuffer(const GLuint* data, size_t count) : m_id(0), m_count(count) {
	glCall<glGenBuffers>(1, &m_id);
	glCall<glBindBuffer>(GL_ELEMENT_ARRAY_BUFFER, m_id);
	glCall<glBufferData>(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(GLuint), data, GL_STATIC_DRAW);
}

IndexBuffer::~IndexBuffer() {
	glDeleteBuffers(1, &m_id);
}

IndexBuffer::IndexBuffer(IndexBuffer&& other) noexcept {
	*this = std::move(other);
}

IndexBuffer& IndexBuffer::operator=(IndexBuffer&& other) noexcept {
	glDeleteBuffers(1, &m_id);

	m_id = other.m_id;
	m_count = other.m_count;

	other.m_id = 0;
	other.m_count = 0;

	return *this;
}

void IndexBuffer::bind() const {
	glCall<glBindBuffer>(GL_ELEMENT_ARRAY_BUFFER, m_id);
}

void IndexBuffer::unbind() {
	glCall<glBindBuffer>(GL_ELEMENT_ARRAY_BUFFER, 0);
}
