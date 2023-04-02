#include "vertexarray.h"

#include <glad/glad.h>

#include "constants.h"
#include "renderer/vertexbuffer.h"
#include "log.h"

namespace renderer::gl {

VertexArray::VertexArray(std::vector<std::unique_ptr<VertexBuffer>>&& vb,
                         std::unique_ptr<IndexBuffer>&& ib) {
	glGenVertexArrays(consts::vertex_array_count_one, &id_);
	// LOG_TRACE("VAO id={} created", id_);
	SetBuffers(std::move(vb));
	SetIndexBuffer(std::move(ib));
}

VertexArray::~VertexArray() {
	// LOG_TRACE("VAO id={} deleted", id_);
	glDeleteVertexArrays(consts::vertex_array_count_one, &id_);
}

VertexArray::VertexArray(VertexArray&& other) noexcept {
	*this = std::move(other);
}

VertexArray& VertexArray::operator=(VertexArray&& other) noexcept {
	if (this == &other)
		return *this;
	// Delete owned array
	// LOG_TRACE("VAO id={} delete", id_);
	glDeleteVertexArrays(consts::vertex_array_count_one, &id_);
	// LOG_TRACE("VAO id={} deleted", id_);
	// Assign moving array data
	id_ = other.id_;
	SetBuffers(std::move(other.vbos_));
	SetIndexBuffer(std::move(other.ibo_));
	// Invalidate moving array
	other.id_ = 0;
	return *this;
}

void VertexArray::SetBuffers(std::vector<std::unique_ptr<VertexBuffer>>&& vbs) {
	Bind();

	for (auto vb_it = vbs.begin(); vb_it != vbs.end(); ++vb_it) {
		VertexBuffer& vb = **vb_it;
		vb.Bind();
		const auto& layout = vb.layout();
		char* offset = nullptr;  // char* is compromise between conversion to void*
														// and pointer arithmetic
		const auto& elements = layout.elements();
		for (const auto& element : elements) {
			glEnableVertexAttribArray(element.location);
			glVertexAttribPointer(element.location, element.count, element.type,
														element.normalized, layout.stride(), offset);
			offset += element.count * element.size();
		}
		vb.Unbind();
	}
	Unbind();
	vbos_ = std::move(vbs);
}

void VertexArray::SetIndexBuffer(std::unique_ptr<IndexBuffer>&& ib) {
	Bind();
	ib->Bind();
	Unbind();
	ib->Unbind();
	ibo_ = std::move(ib);
}

void VertexArray::Bind() const { 
	// LOG_TRACE("VAO id={} binded", id_);
	glBindVertexArray(id_); 
}

void VertexArray::Unbind() const { 
	// LOG_TRACE("VAO id={} unbinded", id_);
	glBindVertexArray(0); 
}

}  // namespace renderer::gl
