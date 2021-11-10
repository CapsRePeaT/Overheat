#include "vertexbuffer.h"

#include "platform/opengl/vertexbuffer.h"
#include "renderer_api.h"

std::unique_ptr<VertexBuffer> VertexBuffer::Create(size_t size) {
	switch (RendererAPI::instance()->api()) {
		case RendererAPI::API::None:
			assert(false);
			return nullptr;
		case RendererAPI::API::OpenGL:
			return std::make_unique<OpenGLVertexBuffer>(size);
	}
	assert(false);
	return nullptr;
}

std::unique_ptr<VertexBuffer> VertexBuffer::Create(size_t size,
                                                   const void* data) {
	switch (RendererAPI::instance()->api()) {
		case RendererAPI::API::None:
			assert(false && "Renderer::API::None is not currently supported");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return std::make_unique<OpenGLVertexBuffer>(size, data);
	}
	assert(false);
	return nullptr;
}
