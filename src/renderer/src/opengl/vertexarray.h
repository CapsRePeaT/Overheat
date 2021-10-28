#pragma once

#include <glbinding/gl/types.h>

class VertexBufferLayout;
class VertexBuffer;

class VertexArray
{
public:
	VertexArray();
	~VertexArray();

	VertexArray(VertexArray&& other) noexcept ;
	VertexArray& operator=(VertexArray&& other) noexcept;


	VertexArray(const VertexArray&) = delete;
	VertexArray& operator=(const VertexArray&) = delete;

	void addBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);

	void bind() const;
	static void unbind();

private:
	gl::GLuint m_id{};

	static gl::GLuint boundId;
};
