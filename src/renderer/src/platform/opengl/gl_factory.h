#pragma once

#include <memory>

#include "renderer/i_renderer_factory.h"

namespace renderer {
namespace gl {

class Factory : public IRendererFactory {
	std::unique_ptr<IndexBuffer> NewIndexBuffer(const uint32_t* data,
	                                            size_t size) final;
	
	std::unique_ptr<VertexArray> NewVertexArray() final;
	std::unique_ptr<VertexArray> NewVertexArray(
			std::unique_ptr<VertexBuffer>&& vb,
			std::unique_ptr<IndexBuffer>&& ib) final;
	// Initialization of dynamic buffer
	
	std::unique_ptr<VertexBuffer> NewVertexBuffer(size_t size) final;
	// Initialization of static buffer
	std::unique_ptr<VertexBuffer> NewVertexBuffer(
			const void* data, size_t size,
			std::unique_ptr<VertexBufferLayout>&& layout) final;
};

}  // namespace gl
}  // namespace renderer
