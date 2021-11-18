#pragma once

#include <memory>

#include "indexbuffer.h"
#include "vertexbuffer.h"

class VertexArray {
 public:
	virtual ~VertexArray() = default;
	virtual void SetBuffer(std::unique_ptr<VertexBuffer>&& vb) = 0;
	virtual void Bind() const = 0;
	virtual void Unbind() const = 0;
	virtual void SetIndexBuffer(std::unique_ptr<IndexBuffer>&& ib) = 0;
	[[nodiscard]] virtual const IndexBuffer& indexBuffer() const = 0;
	static std::unique_ptr<VertexArray> Create();
	static std::unique_ptr<VertexArray> Create(std::unique_ptr<VertexBuffer>&& vb,
	                                           std::unique_ptr<IndexBuffer>&& ib);
};
