#pragma once

#include <glbinding/gl/types.h>

class IndexBuffer
{
public:
	IndexBuffer(const gl::GLuint* data, size_t count);
	~IndexBuffer();

	template<size_t size>
	explicit IndexBuffer(const std::array<gl::GLuint, size>& data)
			: IndexBuffer(data.data(), size) { }

	IndexBuffer(IndexBuffer&& other) noexcept;
	IndexBuffer& operator=(IndexBuffer&& other) noexcept;

	IndexBuffer() = delete;
	IndexBuffer(const IndexBuffer&) = delete;
	IndexBuffer& operator=(const IndexBuffer&) = delete;

	void bind() const;
	static void unbind();

	[[nodiscard]] gl::GLuint getCount() const { return m_count; };
private:
	gl::GLuint m_id = 0;
	gl::GLuint m_count = 0;
};
