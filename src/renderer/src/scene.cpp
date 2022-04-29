#include "scene.h"

#include <algorithm>
#include <vector>

#include "application/scene_shape.h"
#include "heatmap_normalizer.h"
#include "renderer/orthographic_camera.h"

namespace renderer {

struct Scene::SceneImpl {
	std::vector<std::shared_ptr<SceneShape>> scene_shapes;
	std::unique_ptr<OrthographicCamera> camera;
	Heatmaps heatmaps;
};

Scene::Scene() : impl_(std::make_unique<SceneImpl>()) {}

Scene::~Scene() = default;

void Scene::AddShape(const std::shared_ptr<BasicShape>& shape) {
	impl_->scene_shapes.emplace_back(std::make_shared<SceneShape>(*shape));
}

void Scene::AddHeatmaps(const HeatmapStorage& heatmaps_storage) {
	HeatmapNormalizer normalizer(
			heatmaps_storage.MinStep(), heatmaps_storage.x_size(),
			heatmaps_storage.y_size(), 16384, heatmaps_storage.x_steps(),
			heatmaps_storage.y_steps());
	const auto& heatmaps = heatmaps_storage.heatmaps();
	impl_->heatmaps.reserve(heatmaps.size());
	std::ranges::transform(
			heatmaps, std::back_inserter(impl_->heatmaps),
			// Possible copying of vectors
			[&normalizer](const auto& h) {
				auto denormalized_heatmap = normalizer.BilinearInterpolate(h);
				return normalizer.Normalize(std::move(denormalized_heatmap));
			});
}

const std::vector<std::shared_ptr<SceneShape>>& Scene::shapes() const {
	return impl_->scene_shapes;
}

const Heatmaps& Scene::heatmaps() const {
	return impl_->heatmaps;
}

void Scene::Clear() { impl_->scene_shapes.clear(); }

}  // namespace renderer
