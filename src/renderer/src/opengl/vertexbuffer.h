#pragma once

#include <glbinding/gl/types.h>
#include <glbinding/gl/enum.h>

class VertexBuffer
{
public:
	using GLenumType = std::underlying_type_t<gl::GLenum>;
	enum BufferType : GLenumType {
		STATIC = static_cast<GLenumType>(gl::GL_STATIC_DRAW),
		DYNAMIC = static_cast<GLenumType>(gl::GL_DYNAMIC_DRAW),
	};

	VertexBuffer(const void* data, size_t size, BufferType bufferType = BufferType::STATIC);
	~VertexBuffer();

	template<typename T, size_t size>
	explicit VertexBuffer(const std::array<T, size>& data, BufferType bufferType = BufferType::STATIC)
			: VertexBuffer(data.data(), size * sizeof(T), bufferType) { }

	VertexBuffer(VertexBuffer&& other) noexcept;
	VertexBuffer& operator=(VertexBuffer&& other) noexcept;

	VertexBuffer() = delete;
	VertexBuffer(const VertexBuffer&) = delete;
	VertexBuffer& operator=(const VertexBuffer&) = delete;

	void bufferSubData(gl::GLsizeiptr size, const void* data, gl::GLintptr offset = 0);

	template<typename T, size_t size>
	inline void bufferSubData(const std::array<T, size>& data, gl::GLintptr offset = 0) {
		bufferSubData(size * sizeof(T), data.data(), offset);
	}

	void bind() const;
	static void unbind();
private:
	gl::GLuint m_id = 0;

	static gl::GLuint boundId;
};
