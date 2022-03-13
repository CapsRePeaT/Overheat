#include "indexbuffer.h"

#include "platform/opengl/indexbuffer.h"
#include "renderer_api.h"

namespace renderer {

std::unique_ptr<IndexBuffer> IndexBuffer::Create(const uint32_t* data,
                                                 const size_t size) {
	switch (RendererAPI::instance().api()) {
		case API::None:
			assert(false && "Renderer::API::None is not currently supported");
			return nullptr;
		case API::OpenGL:
			return std::make_unique<gl::IndexBuffer>(data, size);
		default:
			assert(false && "Unknown Renderer::API");
			return nullptr;
	}
}

}  // namespace renderer
