#pragma once

#include <array>
#include <memory>

#include "renderer/indexbuffer.h"
#include "renderer/vertexarray.h"

namespace renderer {

class IRendererFactory {
 public:
	virtual ~IRendererFactory() = default;
	virtual std::unique_ptr<IndexBuffer> NewIndexBuffer(const uint32_t* data,
	                                                    size_t size) = 0;
	template <size_t size>
	std::unique_ptr<IndexBuffer> NewIndexBuffer(
			const std::array<uint32_t, size>& data) {
		return NewIndexBuffer(data.data(), size);
	}

	virtual std::unique_ptr<VertexArray> NewVertexArray() = 0;
	virtual std::unique_ptr<VertexArray> NewVertexArray(
			std::unique_ptr<VertexBuffer>&& vb,
			std::unique_ptr<IndexBuffer>&& ib) = 0;

	// Initialization of dynamic buffer
	virtual std::unique_ptr<VertexBuffer> NewVertexBuffer(size_t size) = 0;
	// Initialization of static buffer
	virtual std::unique_ptr<VertexBuffer> NewVertexBuffer(
			const void* data, size_t size,
			std::unique_ptr<VertexBufferLayout>&& layout) = 0;
	// Initialization of static buffer
	template <typename T, size_t size>
	std::unique_ptr<VertexBuffer> NewVertexBuffer(
			const std::array<T, size>& data,
			std::unique_ptr<VertexBufferLayout>&& layout) {
		return NewVertexBuffer(data.data(), size * sizeof(T), std::move(layout));
	}
};

}  // namespace renderer
