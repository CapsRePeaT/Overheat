#pragma once

#include <vector>

#include "renderer/vertexarray.h"

class OpenGLVertexArray final : public VertexArray {
 public:
	OpenGLVertexArray(std::unique_ptr<VertexBuffer>&& vb,
	                  std::unique_ptr<IndexBuffer>&& ib);
	OpenGLVertexArray() : OpenGLVertexArray(nullptr, nullptr) {}
	OpenGLVertexArray(OpenGLVertexArray&& other) noexcept;
	OpenGLVertexArray& operator=(OpenGLVertexArray&& other) noexcept;
	~OpenGLVertexArray() override;
	OpenGLVertexArray(const OpenGLVertexArray&) = delete;
	OpenGLVertexArray& operator=(const OpenGLVertexArray&) = delete;
	void SetBuffer(std::unique_ptr<VertexBuffer>&& vb) final;
	void SetIndexBuffer(std::unique_ptr<IndexBuffer>&& ib) final;
	[[nodiscard]] const IndexBuffer& indexBuffer() const final { return *ibo_; }
	void Bind() const final;
	void Unbind() const final;

 private:
	uint32_t id_;
	std::unique_ptr<VertexBuffer> vbo_;
	std::unique_ptr<IndexBuffer> ibo_;
};
