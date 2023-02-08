#include "vertexarray.h"

#include <glad/glad.h>

#include "constants.h"
#include "renderer/vertexbuffer.h"

namespace renderer::gl {

VertexArray::VertexArray(std::unique_ptr<VertexBuffer>&& vb,
                         std::unique_ptr<IndexBuffer>&& ib) {
	glGenVertexArrays(consts::vertex_array_count_one, &id_);
	SetBuffer(std::move(vb));
	SetIndexBuffer(std::move(ib));
}

VertexArray::~VertexArray() {
	glDeleteVertexArrays(consts::vertex_array_count_one, &id_);
}

VertexArray::VertexArray(VertexArray&& other) noexcept {
	*this = std::move(other);
}

VertexArray& VertexArray::operator=(VertexArray&& other) noexcept {
	if (this == &other)
		return *this;
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

void VertexArray::SetBuffer(std::unique_ptr<VertexBuffer>&& vb) {
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
	Unbind();
	vb->Unbind();
	vbo_ = std::move(vb);
}

void VertexArray::SetIndexBuffer(std::unique_ptr<IndexBuffer>&& ib) {
	Bind();
	ib->Bind();
	Unbind();
	ib->Unbind();
	ibo_ = std::move(ib);
}

void VertexArray::Bind() const { glBindVertexArray(id_); }

void VertexArray::Unbind() const { glBindVertexArray(0); }

}  // namespace renderer::gl
