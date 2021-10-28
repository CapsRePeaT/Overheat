#pragma once

#include <glbinding/gl/enum.h>
#include <glbinding/gl/types.h>

#include <vector>

class VertexBufferLayout
{
public:
	struct VertexBufferElement;
	using VertexBufferElements = std::vector<VertexBufferElement>;

	VertexBufferLayout() = default;
	~VertexBufferLayout() = default;

	VertexBufferLayout(VertexBufferLayout&&) = default;
	VertexBufferLayout& operator=(VertexBufferLayout&&) = default;

	VertexBufferLayout(const VertexBufferLayout&) = delete;
	VertexBufferLayout& operator=(const VertexBufferLayout&) = delete;

	template<typename T>
	void push(unsigned int count) {
		m_elements.push_back({count, _glTypeToEnum<T>(), sizeof(T) == 1});
		m_stride += sizeof(T) * count;
	}

	[[nodiscard]] inline size_t getStride() const { return m_stride; }
	[[nodiscard]] inline const VertexBufferElements& getElements() const { return m_elements; }


	struct alignas(16) VertexBufferElement // NOLINT(cppcoreguidelines-avoid-magic-numbers)
	{
		unsigned int count;
		gl::GLenum type;
		gl::GLboolean normalized;

		[[nodiscard]] unsigned int size() const;
	};
private:
	size_t m_stride = 0;
	VertexBufferElements m_elements;

	template<typename T>
	static constexpr gl::GLenum _glTypeToEnum() {
		if constexpr(std::is_same_v<T, gl::GLfloat>)
			return gl::GL_FLOAT;
		else if constexpr(std::is_same_v<T, gl::GLuint>)
			return gl::GL_UNSIGNED_INT;
		else if constexpr(std::is_same_v<T, gl::GLbyte>)
			return gl::GL_UNSIGNED_BYTE;
	}
};
