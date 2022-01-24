#include "i_scene_viewport.h"

#include <memory>

#include "gl_scene_viewport.h"
#include "log.h"

#define CASE_NOT_SUPPORTED(api)                          \
	case (api):                                            \
		LOG_CRITICAL("{} is not currently supported", #api); \
		return nullptr

std::unique_ptr<ISceneViewport> ISceneViewport::Create(
		API api, std::shared_ptr<Scene> scene) {
	switch (api) {
		case ISceneViewport::API::OpenGL:
			return std::make_unique<GLSceneViewport>(std::move(scene));
			CASE_NOT_SUPPORTED(ISceneViewport::API::None);
			CASE_NOT_SUPPORTED(ISceneViewport::API::Vulkan);
			CASE_NOT_SUPPORTED(ISceneViewport::API::D3D);
		default:
			LOG_CRITICAL("Unkown GraphicsAPI");
			return nullptr;
	}
}

#undef CASE_NOT_SUPPORTED
