#include "renderer_api.h"

#include "platform/opengl/renderer_api.h"

// Relatively temporary definition
// Approach will be change on other apis implementation
RendererAPI::API RendererAPI::api_ = API::OpenGL;

std::unique_ptr<RendererAPI> RendererAPI::instance() {
	switch (api_) {
		case API::None:
			assert(false && "Renderer::API::None is not currently supported");
			return nullptr;
		case API::OpenGL:
			return std::make_unique<OpenGLRendererAPI>();
		default:
			assert(false && "Unknown Renderer::API");
			return nullptr;
	}
}
