#pragma once

#include <glad/glad.h>

#include <array>
#include <cstddef>
#include <type_traits>

class VertexBuffer {
 public:
	enum class BufferType : GLenum {
		STATIC = GL_STATIC_DRAW,
		DYNAMIC = GL_DYNAMIC_DRAW,
	};

	VertexBuffer(const void* data, size_t size,
	             BufferType buffer_type = BufferType::STATIC);
	~VertexBuffer();

	template <typename T, size_t size>
	explicit VertexBuffer(const std::array<T, size>& data,
	                      BufferType buffer_type = BufferType::STATIC)
			: VertexBuffer(data.data(), size * sizeof(T), buffer_type) {}

	VertexBuffer(VertexBuffer&& other) noexcept;
	VertexBuffer& operator=(VertexBuffer&& other) noexcept;

	VertexBuffer() = delete;
	VertexBuffer(const VertexBuffer&) = delete;
	VertexBuffer& operator=(const VertexBuffer&) = delete;

	void BufferSubData(GLsizeiptr size, const void* data, GLintptr offset = 0);
	template <typename T, size_t size>
	inline void BufferSubData(const std::array<T, size>& data,
	                          GLintptr offset = 0) {
		BufferSubData(size * sizeof(T), data.data(), offset);
	}

	void Bind() const;
	static void Unbind();

 private:
	uint32_t id_ = 0;
	static uint32_t bound_id_;
};
