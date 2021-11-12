#include "indexbuffer.h"

#include "platform/opengl/indexbuffer.h"
#include "renderer_api.h"

std::unique_ptr<IndexBuffer> IndexBuffer::Create(const uint32_t* data,
                                                 size_t size) {
	switch (RendererAPI::instance()->api()) {
		case RendererAPI::API::None:
			assert(false && "Renderer::API::None is not currently supported");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return std::make_unique<OpenGLIndexBuffer>(data, size);
	}
	assert(false);
	return nullptr;
}
