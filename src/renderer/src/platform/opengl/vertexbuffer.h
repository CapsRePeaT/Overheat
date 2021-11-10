#pragma once

#include "renderer/vertexbuffer.h"

class OpenGLVertexBuffer : public VertexBuffer {
 public:
	OpenGLVertexBuffer(size_t size);
	OpenGLVertexBuffer(size_t size, const void* data);

	~OpenGLVertexBuffer();

	OpenGLVertexBuffer(OpenGLVertexBuffer&& other) noexcept;
	OpenGLVertexBuffer& operator=(OpenGLVertexBuffer&& other) noexcept;

	OpenGLVertexBuffer() = delete;
	OpenGLVertexBuffer(const OpenGLVertexBuffer&) = delete;
	OpenGLVertexBuffer& operator=(const OpenGLVertexBuffer&) = delete;

	void Bind() const final;
	void Unbind() const final;

	void SetData(size_t size, const void* data) final;

 private:
	uint32_t id_ = 0;
	static uint32_t bound_id_;
};
