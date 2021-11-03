#pragma once

#include <glad/glad.h>

#include <memory>
#include <vector>

#include "indexbuffer.h"

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

	void AddBuffer(const std::shared_ptr<VertexBuffer>& vb,
	               const VertexBufferLayout& layout);
	void SetIndexBuffer(const std::shared_ptr<IndexBuffer>& ib) {
		Bind();
		ib->Bind();
		ib_ = ib;
	}

	const IndexBuffer& indexBuffer() const { return *ib_; }

	void Bind() const;
	static void Unbind();

 private:
	uint32_t id_;
	static uint32_t bound_id_;

	std::vector<std::shared_ptr<VertexBuffer>> vbos_;
	std::shared_ptr<IndexBuffer> ib_;
};
