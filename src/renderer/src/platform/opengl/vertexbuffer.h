#pragma once

#include <memory>

#include "renderer/vertexbuffer.h"

namespace renderer::gl {

class VertexBuffer : public renderer::VertexBuffer {
 public:
	VertexBuffer() = delete;
	explicit VertexBuffer(size_t size);
	VertexBuffer(const void* data, size_t size,
	             std::unique_ptr<VertexBufferLayout>&& layout);
	VertexBuffer(VertexBuffer&& other) noexcept;
	VertexBuffer& operator=(VertexBuffer&& other) noexcept;
	~VertexBuffer();
	VertexBuffer(const VertexBuffer&) = delete;
	VertexBuffer& operator=(const VertexBuffer&) = delete;
	void SetLayout(std::unique_ptr<VertexBufferLayout>&& layout) final {
		layout_ = std::move(layout);
	}
	[[nodiscard]] const VertexBufferLayout& layout() const final {
		return *layout_;
	}
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

}  // namespace renderer::gl
