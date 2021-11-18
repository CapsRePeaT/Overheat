#pragma once

#include <memory>

#include "renderer/vertexbuffer.h"

class OpenGLVertexBuffer : public VertexBuffer {
 public:
	OpenGLVertexBuffer() = delete;
	OpenGLVertexBuffer(const size_t size);
	OpenGLVertexBuffer(const void* data, size_t size,
	                   std::unique_ptr<VertexBufferLayout>&& layout);
	OpenGLVertexBuffer(OpenGLVertexBuffer&& other) noexcept;
	OpenGLVertexBuffer& operator=(OpenGLVertexBuffer&& other) noexcept;
	~OpenGLVertexBuffer();
	OpenGLVertexBuffer(const OpenGLVertexBuffer&) = delete;
	OpenGLVertexBuffer& operator=(const OpenGLVertexBuffer&) = delete;
	void SetLayout(std::unique_ptr<VertexBufferLayout>&& layout) final {
		layout_ = std::move(layout);
	}
	[[nodiscard]] const VertexBufferLayout& layout() const final { return *layout_; }
	void Bind() const final;
	void Unbind() const final;
	void SetData(const void* data, size_t size,
	             std::unique_ptr<VertexBufferLayout>&& layout) final;

 private:
	uint32_t id_ = 0;
	// Actually it can be shared between buffers, but its planned to compute
	// layout statically via vertex structs and reflection
	std::unique_ptr<VertexBufferLayout> layout_;
};
