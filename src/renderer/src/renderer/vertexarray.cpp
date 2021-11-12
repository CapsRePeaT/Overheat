#include "vertexarray.h"

#include <memory>

#include "platform/opengl/vertexarray.h"
#include "renderer/renderer_api.h"

std::unique_ptr<VertexArray> VertexArray::Create() {
	switch (RendererAPI::instance()->api()) {
		case RendererAPI::API::None:
			assert(false && "Renderer::API::None is not currently supported");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return std::make_unique<OpenGLVertexArray>();
	}
	assert(false);
	return nullptr;
}

std::unique_ptr<VertexArray> VertexArray::Create(
		std::unique_ptr<VertexBuffer>&& vb, std::unique_ptr<IndexBuffer>&& ib) {
	switch (RendererAPI::instance()->api()) {
		case RendererAPI::API::None:
			assert(false && "Renderer::API::None is not currently supported");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return std::make_unique<OpenGLVertexArray>(std::move(vb), std::move(ib));
	}
	assert(false);
	return nullptr;
}
