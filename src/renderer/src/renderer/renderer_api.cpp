#include "renderer_api.h"

#include <memory>

#include "platform/opengl/gl_factory.h"
#include "platform/opengl/renderer_api.h"

namespace renderer {

namespace {

thread_local std::unique_ptr<RendererAPI> inst = nullptr;
thread_local API using_api = API::None;
thread_local std::unique_ptr<IRendererFactory> using_factory = nullptr;
const API& api_ref = using_api;

}  // namespace

std::unique_ptr<RendererAPI> RendererAPI::createInstance() {
	switch (using_api) {
		case API::None:
			assert(false && "Renderer::API::None is not currently supported");
			return nullptr;
		case API::OpenGL:
			using_factory = std::make_unique<gl::Factory>();
			return std::make_unique<gl::RendererAPI>();
		default:
			assert(false && "Unknown Renderer::API");
			return nullptr;
	}
}

RendererAPI& RendererAPI::Init(API api) {
	thread_local bool is_inited = false;
	is_inited = true;
	using_api = api;
	auto& inst = instance();
	if (!is_inited) {
		inst.InitImpl();
	}
	return inst;
}

API RendererAPI::api() { return api_ref; }

RendererAPI& RendererAPI::instance() {
	thread_local std::unique_ptr<RendererAPI> inst = createInstance();
	return *inst;
}

IRendererFactory& RendererAPI::factory() {
	return *using_factory;
}

}  // namespace renderer
