#include "scene.h"

#include "application/heatmap_material.h"
#include "application/scene_shape.h"
#include "renderer/orthographic_camera.h"

struct Scene::SceneImpl {
	std::vector<std::shared_ptr<SceneShape>> scene_shapes;
	std::unique_ptr<OrthographicCamera> camera;
};

Scene::Scene() : impl_(std::make_unique<SceneImpl>().release()) {}

Scene::Scene(Scene&& other) noexcept { *this = std::move(other); }

Scene& Scene::operator=(Scene&& other) noexcept {
	if (this != &other) {
		delete impl_;
		impl_       = other.impl_;
		other.impl_ = nullptr;
	}
	return *this;
}

Scene::~Scene() { delete impl_; }

void Scene::AddShape(const std::shared_ptr<BasicShape>& shape) {
	impl_->scene_shapes.emplace_back(std::make_shared<SceneShape>(*shape));
}

void Scene::AddHeatmaps(const HeatmapStorage& heatmaps_storage) {}

const std::vector<std::shared_ptr<SceneShape>>& Scene::shapes() const {
	return impl_->scene_shapes;
}

void Scene::Clear() { impl_->scene_shapes.clear(); }
