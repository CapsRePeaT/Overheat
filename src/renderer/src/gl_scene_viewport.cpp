#include "gl_scene_viewport.h"

#include <glad/glad.h>
#include <glm/gtx/rotate_vector.hpp>
#include <spdlog/spdlog.h>

#include <memory>

#include "application/heatmap_material.h"
#include "application/scene_shape.h"
// TODO: maybe rename inner *renderer* folder to *primitives* or smth similar
#include "renderer/orthographic_camera.h"
#include "renderer/renderer_api.h"

struct GLSceneViewport::GLRendererData {
	std::unique_ptr<HeatmapMaterial> heatmap_material;
	std::vector<std::shared_ptr<SceneShape>> scene_shapes;
	std::unique_ptr<OrthographicCamera> camera;
};

GLSceneViewport::GLSceneViewport()
		: data_(std::make_unique<GLRendererData>()) {}

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
	data_->camera = std::make_unique<OrthographicCamera>(
			aspect_ratio, zoom, std::pair{-20.0f, 20.0f});
	data_->camera->SetPosition({0.0f, 0.0f, -5.0f});
	data_->heatmap_material = std::make_unique<HeatmapMaterial>();
	is_initialized_ = true;
}

void GLSceneViewport::ClearResources() { ClearResourcesImpl(); }

void GLSceneViewport::ClearResourcesImpl() {
	data_->heatmap_material.reset();
	data_->scene_shapes.clear();
	spdlog::debug("Context cleared");
}

void GLSceneViewport::RenderFrame() {
	static glm::vec3 rot_axis = {-1.0f, 0.0f, 0.0f};
	const float kRotSpeed = 0.05f;
	if (!data_->scene_shapes.empty()) {
		data_->scene_shapes[0]->Rotate(kRotSpeed, rot_axis);
		rot_axis = glm::rotateZ<float>(rot_axis, kRotSpeed * 0.4);
	}
	auto api = RendererAPI::instance();
	api->Clear();
	for (auto& shape : data_->scene_shapes) {
		data_->heatmap_material->Use(shape->transform(),
		                             data_->camera->viewProjectionMatrix());
		api->DrawIndexed(shape->vertex_array());
	}
}

void GLSceneViewport::Resize(const int w, const int h) {
	RendererAPI::instance()->SetViewPort(0, 0, w, h);
	data_->camera->SetAspectRatio(static_cast<float>(w) / static_cast<float>(h));
}

void GLSceneViewport::AddShape(const std::shared_ptr<BasicShape>& shape) {
	data_->scene_shapes.emplace_back(std::make_shared<SceneShape>(*shape));
}

void GLSceneViewport::SetTemperatureRange(const float min, const float max) {
	data_->heatmap_material->SetTemperatureRange(min, max);
}

void GLSceneViewport::SetColorRange(const ISceneViewport::Color min,
                                    const ISceneViewport::Color max) {
	data_->heatmap_material->SetColorRange({min[0], min[1], min[2]},
	                                       {max[0], max[1], max[2]});
}
void GLSceneViewport::ClearScene() { data_->scene_shapes.clear(); }
