#include "scene.h"

#include <algorithm>
#include <vector>

#include "application/scene_shape.h"
#include "heatmap_normalizer.h"
#include "log.h"
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
	// TODO: get max_resolution from renderer API (but may be leaved as it is
	// because we are in scene and do not know which renderer api to call)
	static constexpr size_t max_renderer_api_resolution = 32; //16384;
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

const Heatmaps& Scene::heatmaps() const { return impl_->heatmaps; }

void Scene::Clear() { impl_->scene_shapes.clear(); }

}  // namespace renderer
