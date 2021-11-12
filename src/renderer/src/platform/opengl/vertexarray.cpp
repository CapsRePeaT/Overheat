#include "vertexarray.h"

#include <glad/glad.h>

#include "renderer/vertexbuffer.h"

OpenGLVertexArray::OpenGLVertexArray(std::unique_ptr<VertexBuffer>&& vb,
                                     std::unique_ptr<IndexBuffer>&& ib) {
	glGenVertexArrays(1, &id_);
	SetBuffer(std::move(vb));
	SetIndexBuffer(std::move(ib));
}

OpenGLVertexArray::~OpenGLVertexArray() { glDeleteVertexArrays(1, &id_); }

OpenGLVertexArray::OpenGLVertexArray(OpenGLVertexArray&& other) noexcept {
	if (this == &other) return;
	*this = std::move(other);
}

OpenGLVertexArray& OpenGLVertexArray::operator=(
		OpenGLVertexArray&& other) noexcept {
	if (this == &other) return *this;
	// Delete owned array
	glDeleteVertexArrays(1, &id_);
	// Assign moving array data
	id_ = other.id_;
	SetBuffer(std::move(other.vbo_));
	SetIndexBuffer(std::move(other.ib_));
	// Invalidate moving array
	other.id_ = 0;

	return *this;
}

void OpenGLVertexArray::SetBuffer(std::unique_ptr<VertexBuffer>&& vb) {
	Bind();
	vb->Bind();
	auto& layout = vb->layout();

	char* offset = nullptr;  // char* is compromise between conversion to void*
	                         // and pointer arithmetic
	const auto& elements = layout.elements();

	for (unsigned int i = 0; i < elements.size(); ++i) {
		const auto& element = elements[i];
		glEnableVertexAttribArray(i);
		glVertexAttribPointer(i, element.count, element.type, element.normalized,
		                      layout.stride(), offset);

		offset += element.count * element.size();
	}

	vbo_ = std::move(vb);
}

void OpenGLVertexArray::SetIndexBuffer(std::unique_ptr<IndexBuffer>&& ib) {
	Bind();
	ib->Bind();
	ib_ = std::move(ib);
}

void OpenGLVertexArray::Bind() const { glBindVertexArray(id_); }

void OpenGLVertexArray::Unbind() const { glBindVertexArray(0); }
