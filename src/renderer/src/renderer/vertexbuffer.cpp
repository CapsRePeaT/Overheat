#include "vertexbuffer.h"

#include "platform/opengl/vertexbuffer.h"
#include "renderer_api.h"

namespace renderer {

std::unique_ptr<VertexBuffer> VertexBuffer::Create(const size_t size) {
	switch (RendererAPI::instance().api()) {
		case API::None:
			assert(false);
			return nullptr;
		case API::OpenGL:
			return std::make_unique<gl::VertexBuffer>(size);
		default:
			assert(false && "Unknown Renderer::API");
			return nullptr;
	}
}

std::unique_ptr<VertexBuffer> VertexBuffer::Create(
		const void* data, const size_t size,
		std::unique_ptr<VertexBufferLayout>&& layout) {
	switch (RendererAPI::instance().api()) {
		case API::None:
			assert(false && "Renderer::API::None is not currently supported");
			return nullptr;
		case API::OpenGL:
			return std::make_unique<gl::VertexBuffer>(data, size, std::move(layout));
		default:
			assert(false && "Unknown Renderer::API");
			return nullptr;
	}
}

}  // namespace renderer
