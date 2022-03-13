#include "renderer_api.h"

#include <memory>

#include "platform/opengl/renderer_api.h"

namespace renderer {

namespace {

thread_local std::unique_ptr<RendererAPI> inst = nullptr;
thread_local API using_api = API::None;
const API& api_ref = using_api;

}  // namespace

std::unique_ptr<RendererAPI> RendererAPI::createInstance() {
	switch (using_api) {
		case API::None:
			assert(false && "Renderer::API::None is not currently supported");
			return nullptr;
		case API::OpenGL:
			return std::make_unique<gl::RendererAPI>();
		default:
			assert(false && "Unknown Renderer::API");
			return nullptr;
	}
}

RendererAPI& RendererAPI::Init(API api) {
	thread_local bool is_inited = false;
	if (!is_inited) {
		is_inited = true;
		using_api = api;
		auto& inst = instance();
		inst.InitImpl();
		return inst;
	}
	return instance();
}

API RendererAPI::api() { return api_ref; }

RendererAPI& RendererAPI::instance() {
	thread_local std::unique_ptr<RendererAPI> inst = createInstance();
	return *inst;
}

}  // namespace renderer
