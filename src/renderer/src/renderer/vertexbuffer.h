#pragma once

#include <cstddef>
#include <memory>

#include "renderer/vertexbufferlayout.h"

namespace renderer {

class VertexBuffer {
 public:
	virtual ~VertexBuffer() = default;
	virtual void Bind() const = 0;
	virtual void Unbind() const = 0;
	virtual void SetData(const void* data, size_t size,
	                     std::unique_ptr<VertexBufferLayout>&& layout) = 0;
	template <typename T, size_t size>
	inline void SetData(const std::array<T, size>& data,
	                    std::unique_ptr<VertexBufferLayout>&& layout) {
		SetData(size * sizeof(T), data.data(), std::move(layout));
	}
	// Will be deleted and statically resolved via reflection and vertex structs
	virtual void SetLayout(std::unique_ptr<VertexBufferLayout>&& layout) = 0;
	[[nodiscard]] virtual const VertexBufferLayout& layout() const = 0;
};

}  // namespace renderer
