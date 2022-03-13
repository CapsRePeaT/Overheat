#include "gl_factory.h"

#include <memory>

#include "platform/opengl/indexbuffer.h"
#include "platform/opengl/vertexarray.h"
#include "platform/opengl/vertexbuffer.h"

namespace renderer {
namespace gl {

std::unique_ptr<renderer::IndexBuffer> Factory::NewIndexBuffer(
		const uint32_t* data, size_t size) {
	return std::make_unique<gl::IndexBuffer>(data, size);
}

std::unique_ptr<renderer::VertexArray> Factory::NewVertexArray() {
	return std::make_unique<gl::VertexArray>();
}

std::unique_ptr<renderer::VertexArray> Factory::NewVertexArray(
		std::unique_ptr<renderer::VertexBuffer>&& vb,
		std::unique_ptr<renderer::IndexBuffer>&& ib) {
	return std::make_unique<gl::VertexArray>(std::move(vb), std::move(ib));
}

// Initialization of dynamic buffer
std::unique_ptr<renderer::VertexBuffer> Factory::NewVertexBuffer(size_t size) {
	return std::make_unique<gl::VertexBuffer>(size);
}

// Initialization of static buffer
std::unique_ptr<renderer::VertexBuffer> Factory::NewVertexBuffer(
		const void* data, size_t size,
		std::unique_ptr<VertexBufferLayout>&& layout) {
	return std::make_unique<gl::VertexBuffer>(data, size, std::move(layout));
}

}  // namespace gl
}  // namespace renderer
