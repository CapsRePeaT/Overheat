#include "renderer_api.h"

#include "platform/opengl/renderer_api.h"

namespace renderer {

// Relatively temporary definition
// Approach will be change on other apis implementation
RendererAPI::API RendererAPI::api_ = API::OpenGL;

std::unique_ptr<RendererAPI> RendererAPI::instance() {
	switch (api_) {
		case RendererAPI::API::None:
			assert(false && "Renderer::API::None is not currently supported");
			return nullptr;
		case RendererAPI::API::OpenGL:
			return std::make_unique<gl::RendererAPI>();
		default:
			assert(false && "Unknown Renderer::API");
			return nullptr;
	}
}

}  // namespace renderer
