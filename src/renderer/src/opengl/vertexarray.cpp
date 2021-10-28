#include <vertexarray.h>

#include <opengllog.h>
#include <vertexbuffer.h>
#include <vertexbufferlayout.h>

using namespace gl; // NOLINT(google-build-using-namespace)

GLuint VertexArray::boundId = 0;

VertexArray::VertexArray() {
	glCall<glGenVertexArrays>(1, &m_id);
}

VertexArray::~VertexArray() {
	if (boundId == m_id)
		boundId = 0;
	glDeleteVertexArrays(1, &m_id);
}

VertexArray::VertexArray(VertexArray&& other) noexcept {
	*this = std::move(other);
}

VertexArray& VertexArray::operator=(VertexArray&& other) noexcept {
	glDeleteVertexArrays(1, &m_id);

	m_id = other.m_id;

	other.m_id = 0;

	return *this;
}

void VertexArray::addBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout) {
	bind();
	vb.bind();

	unsigned int offset = 0;
	const auto& elements = layout.getElements();

	for (unsigned int i = 0; i < elements.size(); ++i) {
		const auto& element = elements[i];
		glCall<glEnableVertexAttribArray>(i);
		glCall<glVertexAttribPointer>(i, element.count, element.type, element.normalized, layout.getStride(),
		                              reinterpret_cast<void*>(offset)); // NOLINT(performance-no-int-to-ptr)

		offset += element.count * element.size();
	}
}

void VertexArray::bind() const {
	if (m_id != boundId) {
		glCall<glBindVertexArray>(m_id);
		boundId = m_id;
	}
}

void VertexArray::unbind() {
	if (boundId != 0) {
		glCall<glBindVertexArray>(0);
		boundId = 0;
	}
}
