#include "gl_renderer.h"

#include <glad/glad.h>
#include <spdlog/spdlog.h>

#include <glm/gtx/rotate_vector.hpp>
#include <memory>

#include "application/heatmap_material.h"
#include "application/scene_shape.h"
#include "renderer/renderer_api.h"
#include "renderer/orthographic_camera.h"

struct GLRenderer::GLRendererData {
	std::unique_ptr<HeatmapMaterial> heatmap_material;
	std::vector<std::shared_ptr<SceneShape>> scene_shapes;
	std::unique_ptr<OrthographicCamera> camera;
};

GLRenderer::GLRenderer() : data_(std::make_unique<GLRendererData>()) {}

GLRenderer::~GLRenderer() { ClearResourcesImpl(); }

void GLRenderer::Initialize(int w, int h) {
	if (!gladLoadGL()) spdlog::error("Failed to initialize opengl functions");

	spdlog::debug("GLAD initialized");
	auto ogl = RendererAPI::instance();
	ogl.Init();
	ogl.SetViewPort(0, 0, w, h);
	ogl.SetClearColor({0.49, 0.49, 0.49, 1});

	float aspect_ratio = static_cast<float>(w) / static_cast<float>(h);
	float zoom = 15.0f;
	spdlog::debug(
			"Initializing camera: w = {}, h = {}, aspect_ratio = {}, zoom = {}", w, h,
			aspect_ratio, zoom);
	data_->camera = std::make_unique<OrthographicCamera>(
			aspect_ratio, zoom, std::pair{-20.0f, 20.0f});
	data_->camera->SetPosition({0.0f, 0.0f, -5.0f});

	data_->heatmap_material = std::make_unique<HeatmapMaterial>();

	is_initialized_ = true;
}
void GLRenderer::ClearResources() { ClearResourcesImpl(); }
void GLRenderer::ClearResourcesImpl() {
	data_->heatmap_material.reset();
	data_->scene_shapes.clear();
	spdlog::debug("Context cleared");
}
void GLRenderer::RenderFrame() {
	static glm::vec3 rot_axis = {-1.0f, 0.0f, 0.0f};
	const float kRotSpeed = 0.05f;

	if (!data_->scene_shapes.empty()) {
		data_->scene_shapes[0]->Rotate(kRotSpeed, rot_axis);
		rot_axis = glm::rotateZ<float>(rot_axis, kRotSpeed * 0.4);
	}

	RendererAPI::instance().Clear();
	for (auto& shape : data_->scene_shapes) {
		data_->heatmap_material->Use(shape->transform(),
		                             data_->camera->viewProjectionMatrix());
		RendererAPI::instance().DrawIndexed(shape->vertexArray());
	}
}

void GLRenderer::Resize(int w, int h) {
	RendererAPI::instance().SetViewPort(0, 0, w, h);
	data_->camera->SetAspectRatio(static_cast<float>(w) / static_cast<float>(h));
}

void GLRenderer::AddShape(const std::shared_ptr<BasicShape>& shape) {
	data_->scene_shapes.emplace_back(std::make_shared<SceneShape>(*shape));
}

void GLRenderer::SetTemperatureRange(float min, float max) {
	data_->heatmap_material->SetTemperatureRange(min, max);
}
void GLRenderer::SetColorRange(IRenderer::Color min, IRenderer::Color max) {
	data_->heatmap_material->SetColorRange({min[0], min[1], min[2]},
	                                       {max[0], max[1], max[2]});
}
void GLRenderer::ClearScene() { data_->scene_shapes.clear(); }
