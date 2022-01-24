#include "gl_scene_viewport.h"

#include <glad/glad.h>
#include <glm/gtx/rotate_vector.hpp>
#include <spdlog/spdlog.h>

#include <memory>

// TODO: maybe rename inner *renderer* folder to *primitives* or smth similar
#include "scene.h"
#include "application/scene_shape.h"
#include "renderer/renderer_api.h"

GLSceneViewport::GLSceneViewport(std::shared_ptr<Scene> scene)
		: scene_(scene) {}

GLSceneViewport::~GLSceneViewport() { ClearResourcesImpl(); }

void GLSceneViewport::Initialize(const int w, const int h) {
	// OpenGL initialization
	if (!gladLoadGL())
		spdlog::error("Failed to initialize opengl functions");
	spdlog::debug("GLAD initialized");
	auto api = RendererAPI::instance();
	api->Init();
	api->SetViewPort(0, 0, w, h);
	api->SetClearColor({0.49, 0.49, 0.49, 1});

	// Application initialization
	const float aspect_ratio = static_cast<float>(w) / static_cast<float>(h);
	const float zoom = 15.0f;
	spdlog::debug(
			"Initializing camera: w = {}, h = {}, aspect_ratio = {}, zoom = {}", w, h,
			aspect_ratio, zoom);
	camera_ = std::make_unique<OrthographicCamera>(
			aspect_ratio, zoom, std::pair{-20.0f, 20.0f});
	camera_->SetPosition({0.0f, 0.0f, -5.0f});
	heatmap_material_ = std::make_unique<HeatmapMaterial>();
	is_initialized_ = true;
}

void GLSceneViewport::ClearResources() { ClearResourcesImpl(); }

void GLSceneViewport::ClearResourcesImpl() {
	heatmap_material_.reset();
	scene_->Clear();
	spdlog::debug("Context cleared");
}

void GLSceneViewport::RenderFrame() {
	// TODO: pass scene to renderer
	static glm::vec3 rot_axis = {-1.0f, 0.0f, 0.0f};
	const float kRotSpeed = 0.05f;
	if (!scene_->shapes().empty()) {
		scene_->shapes()[0]->Rotate(kRotSpeed, rot_axis);
		rot_axis = glm::rotateZ<float>(rot_axis, kRotSpeed * 0.4);
	}
	auto api = RendererAPI::instance();
	api->Clear();
	for (auto& shape : scene_->shapes()) {
		heatmap_material_->Use(shape->transform(),
		                             camera_->viewProjectionMatrix());
		api->DrawIndexed(shape->vertex_array());
	}
}

void GLSceneViewport::Resize(const int w, const int h) {
	RendererAPI::instance()->SetViewPort(0, 0, w, h);
	camera_->SetAspectRatio(static_cast<float>(w) / static_cast<float>(h));
}

void GLSceneViewport::SetTemperatureRange(const float min, const float max) {
	heatmap_material_->SetTemperatureRange(min, max);
}

void GLSceneViewport::SetColorRange(const ISceneViewport::Color min,
                                    const ISceneViewport::Color max) {
	heatmap_material_->SetColorRange({min[0], min[1], min[2]},
	                                       {max[0], max[1], max[2]});
}

void GLSceneViewport::MoveCamera(int x, int y, int dX, int dY) {}
void GLSceneViewport::RotateCamera(int x, int y, int dX, int dY) {}
void GLSceneViewport::ZoomView(float delta) {}
