#include "vertexarray.h"

#include "vertexbuffer.h"
#include "vertexbufferlayout.h"

uint32_t VertexArray::bound_id_ = 0;

VertexArray::VertexArray() { glGenVertexArrays(1, &id_); }

VertexArray::~VertexArray() {
	if (bound_id_ == id_) bound_id_ = 0;
	glDeleteVertexArrays(1, &id_);
}

VertexArray::VertexArray(VertexArray&& other) noexcept {
	*this = std::move(other);
}

VertexArray& VertexArray::operator=(VertexArray&& other) noexcept {
	// Delete owned array
	glDeleteVertexArrays(1, &id_);
	// Assign moving array data
	id_ = other.id_;
	// Invalidate moving array
	other.id_ = 0;

	return *this;
}

void VertexArray::AddBuffer(
		const std::shared_ptr<VertexBuffer>& vb,
                            const VertexBufferLayout& layout) {
	Bind();
	vb->Bind();

	char* offset = nullptr; // char* is compromise between conversion to void* and pointer arithmetic
	const auto& elements = layout.elements();

	for (unsigned int i = 0; i < elements.size(); ++i) {
		const auto& element = elements[i];
		glEnableVertexAttribArray(i);
		glVertexAttribPointer(i, element.count, element.type, element.normalized,
		                      layout.stride(), offset);

		offset += element.count * element.size();
	}

	vbos_.emplace_back(vb);
}

void VertexArray::Bind() const {
	if (id_ != bound_id_) {
		glBindVertexArray(id_);
		bound_id_ = id_;
	}
}

void VertexArray::Unbind() {
	if (bound_id_ != 0) {
		glBindVertexArray(0);
		bound_id_ = 0;
	}
}
