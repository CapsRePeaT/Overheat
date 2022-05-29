#include "scene.h"

#include <algorithm>
#include <cassert>
#include <map>
#include <memory>
#include <vector>

#include "application/scene_shape.h"
#include "heatmap_normalizer.h"
#include "log.h"
#include "renderer/orthographic_camera.h"

namespace renderer {

struct Scene::SceneImpl {
	std::vector<std::shared_ptr<SceneShape>> scene_shapes;
	std::map<GlobalId, std::shared_ptr<SceneShape>> indexed_shapes{};
	std::unique_ptr<OrthographicCamera> camera;
	Heatmaps heatmaps;
	std::pair<float, float> bounds;
};

Scene::Scene() : impl_(std::make_unique<SceneImpl>()) {}

Scene::~Scene() = default;

void Scene::AddShape(const std::shared_ptr<BasicShape>& shape) {
	auto index_it = impl_->indexed_shapes.find(shape->id());
	if (index_it != impl_->indexed_shapes.end()) {
		assert(false && "Try to add already added shape to scene");
		return;
	}
	auto new_scene_shape = std::make_shared<SceneShape>(*shape);
	impl_->scene_shapes.emplace_back(new_scene_shape);
	impl_->indexed_shapes[shape->id()] = new_scene_shape;
}

void Scene::AddHeatmaps(const HeatmapStorage& heatmaps_storage) {
	assert(impl_ && "SceneImpl is not initialized");
	impl_->bounds = {heatmaps_storage.x_size(), heatmaps_storage.y_size()};

	// TODO: get max_resolution from renderer API (but may be leaved as it is
	// because we are in scene and do not know which renderer api to call)
	static constexpr size_t max_renderer_api_resolution = 512;  // 16384;
	HeatmapNormalizer normalizer(heatmaps_storage, max_renderer_api_resolution);

	const auto& heatmaps = heatmaps_storage.heatmaps();
	impl_->heatmaps.reserve(heatmaps.size());
	std::ranges::transform(heatmaps, std::back_inserter(impl_->heatmaps),
	                       // Possible copying of vectors
	                       [&normalizer](const auto& h) {
													 auto heatmap = normalizer.BilinearInterpolateSlow(h);
													 normalizer.Normalize(heatmap);
													 return heatmap;
												 });
	LOG_TRACE("Added {} heatmaps", impl_->heatmaps.size());
	LOG_TRACE("Current scene_shape count: {}", impl_->scene_shapes.size());
}

const std::vector<std::shared_ptr<SceneShape>>& Scene::shapes() const {
	return impl_->scene_shapes;
}
[[nodiscard]] std::vector<std::shared_ptr<SceneShape>>& Scene::shapes() {
	return impl_->scene_shapes;
}

const Heatmaps& Scene::heatmaps() const { return impl_->heatmaps; }

std::pair<float, float> Scene::bounds() const { return impl_->bounds; }

void Scene::Clear() {
	impl_->scene_shapes.clear();
	impl_->indexed_shapes.clear();
}

const std::shared_ptr<SceneShape>& Scene::shape_by_id(GlobalId id) const {
	return impl_->indexed_shapes.at(id);
}
std::shared_ptr<SceneShape>& Scene::shape_by_id(GlobalId id) {
	return const_cast<std::shared_ptr<SceneShape>&>(
			const_cast<const Scene*>(this)->shape_by_id(id));
}

}  // namespace renderer
