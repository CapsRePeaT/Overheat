#pragma once

#include <glad/glad.h>

class VertexBufferLayout;
class VertexBuffer;

class VertexArray {
 public:
	VertexArray();
	~VertexArray();

	VertexArray(VertexArray&& other) noexcept;
	VertexArray& operator=(VertexArray&& other) noexcept;

	VertexArray(const VertexArray&) = delete;
	VertexArray& operator=(const VertexArray&) = delete;

	void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);

	void Bind() const;
	static void Unbind();

 private:
	uint32_t id_;
	static uint32_t bound_id_;
};
