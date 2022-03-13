#include "vertexarray.h"

#include <memory>

#include "platform/opengl/vertexarray.h"
#include "renderer/renderer_api.h"

namespace renderer {

std::unique_ptr<VertexArray> VertexArray::Create() {
	switch (RendererAPI::instance()->api()) {
		case RendererAPI::API::None:
			assert(false && "Renderer::API::None is not currently supported");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return std::make_unique<gl::VertexArray>();
		default:
			assert(false && "Unknown Renderer::API");
			return nullptr;
	}
}

std::unique_ptr<VertexArray> VertexArray::Create(
		std::unique_ptr<VertexBuffer>&& vb, std::unique_ptr<IndexBuffer>&& ib) {
	switch (RendererAPI::instance()->api()) {
		case RendererAPI::API::None:
			assert(false && "Renderer::API::None is not currently supported");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return std::make_unique<gl::VertexArray>(std::move(vb), std::move(ib));
		default:
			assert(false && "Unknown Renderer::API");
			return nullptr;
	}
}

}  // namespace renderer
