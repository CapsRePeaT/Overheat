#pragma once

#include <array>
#include <cstddef>
#include <memory>

class VertexBuffer {
 public:
	virtual ~VertexBuffer() = default;
	virtual void Bind() const = 0;
	virtual void Unbind() const = 0;

	virtual void SetData(size_t size, const void* data) = 0;
	template <typename T, size_t size>
	inline void SetData(const std::array<T, size>& data) {
		SetData(size * sizeof(T), data.data());
	}

	// Initialization of dynamic buffer
	static std::unique_ptr<VertexBuffer> Create(size_t size);
	// Initialization of static buffer
	static std::unique_ptr<VertexBuffer> Create(size_t size, const void* data);
	// Initialization of static buffer
	template <typename T, size_t size>
	static std::unique_ptr<VertexBuffer> Create(const std::array<T, size>& data) {
		return Create(size * sizeof(T), data.data());
	}
};
