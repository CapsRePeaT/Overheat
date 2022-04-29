#include "gl_scene_viewport.h"

#include <glad/glad.h>
#include <spdlog/spdlog.h>

#include <memory>

// TODO: maybe rename inner *renderer* folder to *primitives* or smth similar
#include "application/scene_shape.h"
#include "camera_controller.h"
#include "constants.h"
#include "log.h"
#include "misc/formatters.h"
#include "renderer/debug/axes.h"
#include "renderer/debug/debug_heatmap_material.h"
#include "renderer/debug/debug_material.h"
#include "renderer/i_camera.h"
#include "renderer/orthographic_camera.h"
#include "renderer/renderer_api.h"
#include "scene.h"

namespace renderer {

struct GLSceneViewport::Impl {
	std::unique_ptr<debug::DebugMaterial> debug_material =
			std::make_unique<debug::DebugMaterial>();
	std::unique_ptr<debug::Axes> axes = std::make_unique<debug::Axes>();
};

GLSceneViewport::GLSceneViewport(std::shared_ptr<Scene> scene)
		: scene_(std::move(scene)), data_(nullptr) {}

GLSceneViewport::~GLSceneViewport() { ClearResourcesImpl(); }

void GLSceneViewport::Initialize(const int w, const int h) {
	OpenGlInit(w, h);
	ApplicationInit(w, h);
	//#ifndef NDEBUG
	//	DebugInit(w, h);
	//#endif

	is_initialized_ = true;
}

void GLSceneViewport::OpenGlInit(const int w, const int h) {
	if (!gladLoadGL())
		LOG_ERROR("Failed to initialize opengl functions");
	LOG_DEBUG("GLAD initialized");

	auto& api = RendererAPI::Init(API::OpenGL);
	api.SetClearColor(consts::init::clear_color);
	api.SetViewPort(0, 0, w, h);
}

void GLSceneViewport::ApplicationInit(const int w, const int h) {
	const float aspect_ratio = static_cast<float>(w) / static_cast<float>(h);

	LOG_DEBUG("Initializing camera: w = {}, h = {}, aspect_ratio = {}, zoom = {}",
	          w, h, aspect_ratio, consts::init::zoom);

	auto camera = std::make_unique<OrthographicCamera>(
			aspect_ratio, consts::init::zoom, consts::init::near_far_bounds);
	camera_controller_ = std::make_unique<SphericalCameraController>(
			std::move(camera), 100.0f, glm::pi<float>());
	heatmap_material_ = std::make_unique<DebugHeatmapMaterial>();
}

void GLSceneViewport::DebugInit(const int /*w*/, const int /*h*/) {
	data_ = std::make_unique<Impl>();
	data_->axes->ApplyScale(4);
	const Core::Shapes shapes = {
			std::make_shared<BasicShape>(
			GlobalId(InstanceType::Shape, 0, 0), 0,
			Box3D({{0.0f, 1.0f}, {0.0f, 2.0f}, {0.0f, 3.0f}}
             ))
  };
	scene_->AddShapes(shapes);
	constexpr glm::vec3 offset = {0.1, 0.1, 0.1};
	scene_->shapes()[0]->Translate(offset);
}

void GLSceneViewport::ClearResources() { ClearResourcesImpl(); }

void GLSceneViewport::ClearResourcesImpl() {
	heatmap_material_.reset();
	scene_->Clear();
	LOG_DEBUG("Context cleared");
}

void GLSceneViewport::RenderFrame() {
	auto& api = RendererAPI::instance();
	api.Clear();

	const ICamera& camera = camera_controller_->camera();

	for (const auto& shape : scene_->shapes()) {
		heatmap_material_->Use(shape->transform(), camera.viewProjectionMatrix());
		api.DrawIndexed(shape->vertex_array());
	}

	if (data_) {
		const auto& axes = data_->axes;
		data_->debug_material->Use(axes->transform(), camera.viewProjectionMatrix(),
		                           1.0f);
		api.DrawIndexed(axes->vertex_array(), PrimitiveType::LINES);
	}
}

void GLSceneViewport::Resize(const int w, const int h) {
	RendererAPI::instance().SetViewPort(0, 0, w, h);
	camera_controller_->SetCameraAspectRatio(static_cast<float>(w) /
	                                         static_cast<float>(h));
}

void GLSceneViewport::SetTemperatureRange(const float min, const float max) {
	heatmap_material_->SetTemperatureRange(min, max);
}

void GLSceneViewport::SetColorRange(const ISceneViewport::Color min,
                                    const ISceneViewport::Color max) {
	heatmap_material_->SetColorRange({min[0], min[1], min[2]},
	                                 {max[0], max[1], max[2]});
}

void GLSceneViewport::MoveCamera(const Vec2D /*screenPoint*/,
                                 const Vec2D delta) {
	static constexpr float zoom_coefficient = 0.002f;
	const float zoom_level = camera_controller_->camera().zoom_level();

	const float x_coefficient = -zoom_level * zoom_coefficient;
	const float y_coefficient = zoom_level * zoom_coefficient;

	const auto dx = static_cast<float>(delta.x);
	const auto dy = static_cast<float>(delta.y);

	const auto horizontal_translation =
			x_coefficient * dx * camera_controller_->camera().rightVector();
	const auto vertical_translation =
			y_coefficient * dy * camera_controller_->camera().upVector();

	camera_controller_->Translate(horizontal_translation + vertical_translation);
}

void GLSceneViewport::RotateCamera(const Vec2D /*screenPoint*/,
                                   const Vec2D delta) {
	static constexpr float zoom_coefficient = 1.0f / 1500.0f;
	const float zoom_level = camera_controller_->camera().zoom_level();

	const float y_coefficient = zoom_level * zoom_coefficient;
	const float x_coefficient = -zoom_level * zoom_coefficient;

	const auto dx = static_cast<float>(delta.x);
	const auto dy = static_cast<float>(delta.y);

	camera_controller_->AddLongitude(x_coefficient * dx);
	camera_controller_->AddLatitude(y_coefficient * dy);
}

void GLSceneViewport::ZoomView(const float delta) {
	static constexpr float zoom_base = 2.0f;
	static constexpr float zoom_delta_coefficient = -1.0f / 180.0f;
	const float zoom = std::pow(zoom_base, delta * zoom_delta_coefficient);

	camera_controller_->Zoom(zoom);
}

}  // namespace renderer
