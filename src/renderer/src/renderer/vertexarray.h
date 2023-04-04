#pragma once

#include <memory>

#include "indexbuffer.h"
#include "vertexbuffer.h"

namespace renderer {

class VertexArray {
 public:
	virtual ~VertexArray() = default;
	virtual void SetBuffers(std::vector<std::unique_ptr<VertexBuffer>>&& vb) = 0;
	virtual void Bind() const = 0;
	virtual void Unbind() const = 0;
	virtual void SetIndexBuffer(std::unique_ptr<IndexBuffer>&& ib) = 0;
	[[nodiscard]] virtual const IndexBuffer& indexBuffer() const = 0;
};

}  // namespace renderer
