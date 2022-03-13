#include "i_scene_viewport.h"

#include <memory>

#include "gl_scene_viewport.h"
#include "log.h"

#define CASE_NOT_SUPPORTED(api)                          \
	case (api):                                            \
		LOG_CRITICAL("{} is not currently supported", #api); \
		return nullptr

namespace renderer {

std::unique_ptr<ISceneViewport> ISceneViewport::Create(
		API api, std::shared_ptr<Scene> scene) {
	switch (api) {
		case API::OpenGL:
			return std::make_unique<GLSceneViewport>(std::move(scene));
			CASE_NOT_SUPPORTED(API::None);
			CASE_NOT_SUPPORTED(API::Vulkan);
			CASE_NOT_SUPPORTED(API::D3D);
		default:
			LOG_CRITICAL("Unkown GraphicsAPI");
			return nullptr;
	}
}

#undef CASE_NOT_SUPPORTED

}  // namespace renderer
