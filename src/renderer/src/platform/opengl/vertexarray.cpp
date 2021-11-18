#include "vertexarray.h"

#include <glad/glad.h>

#include "constants.h"
#include "renderer/vertexbuffer.h"

OpenGLVertexArray::OpenGLVertexArray(std::unique_ptr<VertexBuffer>&& vb,
                                     std::unique_ptr<IndexBuffer>&& ib) {
	glGenVertexArrays(consts::vertex_array_count_one, &id_);
	SetBuffer(std::move(vb));
	SetIndexBuffer(std::move(ib));
}

OpenGLVertexArray::~OpenGLVertexArray() {
	glDeleteVertexArrays(consts::vertex_array_count_one, &id_);
}

OpenGLVertexArray::OpenGLVertexArray(OpenGLVertexArray&& other) noexcept {
	*this = std::move(other);
}

OpenGLVertexArray& OpenGLVertexArray::operator=(
		OpenGLVertexArray&& other) noexcept {
	if (this == &other) return *this;
	// Delete owned array
	glDeleteVertexArrays(consts::vertex_array_count_one, &id_);
	// Assign moving array data
	id_ = other.id_;
	SetBuffer(std::move(other.vbo_));
	SetIndexBuffer(std::move(other.ibo_));
	// Invalidate moving array
	other.id_ = 0;
	return *this;
}

void OpenGLVertexArray::SetBuffer(std::unique_ptr<VertexBuffer>&& vb) {
	Bind();
	vb->Bind();
	const auto& layout = vb->layout();
	char* offset = nullptr;  // char* is compromise between conversion to void*
	                         // and pointer arithmetic
	const auto& elements = layout.elements();
	for (const auto& element : elements) {
		glEnableVertexAttribArray(element.location);
		glVertexAttribPointer(element.location, element.count, element.type,
		                      element.normalized, layout.stride(), offset);
		offset += element.count * element.size();
	}
	vbo_ = std::move(vb);
}

void OpenGLVertexArray::SetIndexBuffer(std::unique_ptr<IndexBuffer>&& ib) {
	Bind();
	ib->Bind();
	ibo_ = std::move(ib);
}

void OpenGLVertexArray::Bind() const { glBindVertexArray(id_); }

void OpenGLVertexArray::Unbind() const { glBindVertexArray(0); }
