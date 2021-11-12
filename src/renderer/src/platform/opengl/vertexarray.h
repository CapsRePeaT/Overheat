#pragma once

#include <vector>

#include "renderer/vertexarray.h"

class OpenGLVertexArray : public VertexArray {
 public:
	OpenGLVertexArray(std::unique_ptr<VertexBuffer>&& vb,
	                  std::unique_ptr<IndexBuffer>&& ib);
	OpenGLVertexArray() : OpenGLVertexArray(nullptr, nullptr) {}
	~OpenGLVertexArray();

	OpenGLVertexArray(OpenGLVertexArray&& other) noexcept;
	OpenGLVertexArray& operator=(OpenGLVertexArray&& other) noexcept;

	OpenGLVertexArray(const OpenGLVertexArray&) = delete;
	OpenGLVertexArray& operator=(const OpenGLVertexArray&) = delete;

	void SetBuffer(std::unique_ptr<VertexBuffer>&& vb) final;

	void SetIndexBuffer(std::unique_ptr<IndexBuffer>&& ib) final;
	const IndexBuffer& indexBuffer() const final { return *ib_; }

	void Bind() const final;
	void Unbind() const final;

 private:
	uint32_t id_;
	std::unique_ptr<VertexBuffer> vbo_;
	std::unique_ptr<IndexBuffer> ib_;
};
