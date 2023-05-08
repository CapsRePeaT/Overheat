#include "scene.h"

#include <algorithm>
#include <cassert>
#include <map>
#include <memory>
#include <vector>

#include "application/box_shape.h"
#include "application/tetrahedron_shape.h"
#include "heatmap_normalizer.h"
#include "log.h"
#include "renderer/orthographic_camera.h"
#include "renderer/renderer_api.h"

namespace renderer {

struct Scene::SceneImpl {
	std::vector<std::shared_ptr<BoxShape>> scene_shapes;
	std::map<GlobalId, std::shared_ptr<BoxShape>> indexed_shapes{};
	std::unique_ptr<OrthographicCamera> camera;
	Heatmaps heatmaps;
	// move to resource manager?
	std::optional<std::vector<HeatmapMaterial>> heatmap_materials;
	std::pair<float, float> bounds;

	std::vector<glm::vec3> tetrahedron_points_array;
	std::shared_ptr<VertexBuffer> tetrahedron_points;
	std::shared_ptr<VertexBuffer> tetrahedron_temps;
	std::map<GlobalId, std::vector<std::shared_ptr<SolverTetraeder>>>
			indexed_tetrahedrons;
	std::map<GlobalId, std::shared_ptr<TetrahedronShape>>
			indexed_tetrahedron_shapes;
	std::unique_ptr<TetrahedronMaterial> tetrahedron_material;

	std::vector<Drawable*> all_shapes;
};

Scene::Scene() : impl_(std::make_unique<SceneImpl>()) {}

Scene::~Scene() = default;

void Scene::AddShape(const std::shared_ptr<BasicShape>& shape) {
	auto index_it = impl_->indexed_shapes.find(shape->id());
	if (index_it != impl_->indexed_shapes.end()) {
		assert(false && "Try to add already added shape to scene");
		return;
	}
	auto new_scene_shape = std::make_shared<BoxShape>(*shape);
	impl_->scene_shapes.emplace_back(new_scene_shape);
	impl_->indexed_shapes[shape->id()] = new_scene_shape;
	impl_->all_shapes.emplace_back(new_scene_shape.get());
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

bool Scene::UpdateForRenderer() {
	if (use_layered_heatmaps_ && !impl_->heatmaps.empty() &&
	    !impl_->heatmap_materials) {
		InitHeatmapMaterials();
		return true;
	}
	return false;
}

const std::vector<Drawable*>& Scene::shapes() const {
	return impl_->all_shapes;
}

std::pair<float, float> Scene::bounds() const { return impl_->bounds; }

void Scene::Clear() {
	impl_ = std::make_unique<SceneImpl>();
	// impl_->scene_shapes.clear();
	// impl_->indexed_shapes.clear();
	// impl_->heatmaps.clear();
	// impl_->heatmap_materials.reset();
}

void Scene::AddFileRepresentation(FileRepresentation& file_representation,
                                  bool use_layered_heatmaps) {
	use_layered_heatmaps_ = use_layered_heatmaps;
	if (use_layered_heatmaps_) {
		AddShapes(file_representation.GetShapes());
		AddHeatmaps(file_representation.heatmaps());
	} else {
		const auto& fs     = file_representation.fs_datapack();
		const auto& coords = fs.indeces().coords();
		auto& float_coords = impl_->tetrahedron_points_array;
		float_coords.reserve(coords.size());
		for (auto d_coord : coords) {
			float_coords.push_back(glm::vec3(d_coord.coords[0], d_coord.coords[1], d_coord.coords[2]));
		}

		auto& factory = RendererAPI::factory();
		auto layout   = std::make_unique<VertexBufferLayout>();
		layout->Push<float>(3);
		impl_->tetrahedron_points = factory.NewVertexBuffer(
				float_coords.data(), float_coords.size() * sizeof(glm::vec3),
				std::move(layout));

		layout = std::make_unique<VertexBufferLayout>();
		layout->Push<float>(1);
		const auto& temps        = fs.heatmap().temperatures();
		impl_->tetrahedron_temps = factory.NewVertexBuffer(
				temps.data(), temps.size() * sizeof(float), std::move(layout));
		impl_->tetrahedron_material = std::make_unique<TetrahedronMaterial>();

		AddTetrahedrons(fs.elements());
		BuildTetrahedronShapes();
	}
}

std::shared_ptr<Drawable> Scene::shape_by_id(GlobalId id) const {
	auto box_shape_it = impl_->indexed_shapes.find(id);
	if (box_shape_it != impl_->indexed_shapes.end())
		return box_shape_it->second;

	auto tetra_shape_it = impl_->indexed_tetrahedron_shapes.find(id);
	if (tetra_shape_it != impl_->indexed_tetrahedron_shapes.end())
		return tetra_shape_it->second;
	
	return nullptr;
}

void Scene::SetTemperatureRange(const float min, const float max) {
	if (impl_->heatmap_materials) {
		auto& materials = *impl_->heatmap_materials;
		for (auto& material : materials) material.SetTemperatureRange(min, max);
	}
	if (impl_->tetrahedron_material) {
		impl_->tetrahedron_material->SetTemperatureRange(min, max);
	}
}

void Scene::SetColors(const std::array<glm::vec3, 5>& colors) {
	if (impl_->heatmap_materials) {
		auto& materials = *impl_->heatmap_materials;
		for (auto& material : materials) material.SetColors(colors);
	}
	if (impl_->tetrahedron_material) {
		impl_->tetrahedron_material->SetColors(colors);
	}
}

void Scene::SetDrawMode(const DrawMode mode) {
	bool is_stratified = false;
	switch (mode) {
		case DrawMode::Gradient:
			is_stratified = false;
			break;
		case DrawMode::Stratified:
			is_stratified = true;
			break;
	}
	if (impl_->heatmap_materials) {
		auto& materials = *impl_->heatmap_materials;
		for (auto& material : materials) material.SetIsStratified(is_stratified);
	}
	if (impl_->tetrahedron_material) {
		impl_->tetrahedron_material->SetIsStratified(is_stratified);
	}
}

void Scene::SetStratifiedStep(float step) {
	if (impl_->heatmap_materials) {
		auto& materials = *impl_->heatmap_materials;
		for (auto& material : materials) material.SetStratifiedStep(step);
	}
	if (impl_->tetrahedron_material) {
		impl_->tetrahedron_material->SetStratifiedStep(step);
	}
}

void Scene::SetVisibility(const GlobalIds& to_change, bool is_visible) {
	for (auto id : to_change) {
		auto shape_it = impl_->indexed_shapes.find(id);
		if (shape_it != impl_->indexed_shapes.end())
			shape_it->second->SetIsVisible(is_visible);

		auto tetra_it = impl_->indexed_tetrahedron_shapes.find(id);
		if (tetra_it != impl_->indexed_tetrahedron_shapes.end())
			tetra_it->second->SetIsVisible(is_visible);
	}
}

HeatmapMaterial CreateMaterial(const Heatmap& bot_heatmap,
                               const Heatmap& top_heatmap,
                               std::pair<float, float> bounds) {
	auto& factory               = RendererAPI::factory();
	int side_resolution         = bot_heatmap.x_resolution();
	auto heatmap_bottom_texture = factory.NewTexture2D(
			side_resolution, side_resolution, bot_heatmap.temperatures().data(), 1);
	auto heatmap_top_texture = factory.NewTexture2D(
			side_resolution, side_resolution, top_heatmap.temperatures().data(), 1);

	auto texture_pair =
			std::pair<std::unique_ptr<Texture2D>, std::unique_ptr<Texture2D>>(
					std::move(heatmap_bottom_texture), std::move(heatmap_top_texture));
	auto temp_ranges_pair = std::pair<glm::vec2, glm::vec2>(
			{bot_heatmap.min_temp(), bot_heatmap.max_temp()},
			{top_heatmap.min_temp(), top_heatmap.max_temp()});

	return {std::move(texture_pair), temp_ranges_pair, bounds};
}

void Scene::InitHeatmapMaterials() {
	auto& heatmaps_ = impl_->heatmaps;
	assert(heatmaps_.size());
	impl_->heatmap_materials = std::vector<HeatmapMaterial>();
	auto& heatmap_materials  = *impl_->heatmap_materials;
	heatmap_materials.reserve(heatmaps_.size() - 1);
	if (heatmaps_.size() == 1) {
		const auto& heatmap = heatmaps_.front();
		heatmap_materials.emplace_back(CreateMaterial(heatmap, heatmap, bounds()));
	}
	for (size_t i = 0; i < heatmaps_.size() - 1; ++i) {
		const auto& bot_heatmap = heatmaps_[i];
		const auto& top_heatmap = heatmaps_[i + 1];
		heatmap_materials.emplace_back(
				CreateMaterial(bot_heatmap, top_heatmap, bounds()));
	}

	for (auto& shape : impl_->scene_shapes) {
		shape->SetMaterial(heatmap_materials[shape->layer_id()]);
	}
}

void Scene::BuildTetrahedronShapes() {
	for (auto& [id, tetrahedrons] : impl_->indexed_tetrahedrons) {
		auto new_shape = std::make_shared<TetrahedronShape>(
				tetrahedrons, impl_->tetrahedron_points, impl_->tetrahedron_temps,
				*impl_->tetrahedron_material, impl_->tetrahedron_points_array);
		impl_->indexed_tetrahedron_shapes.emplace(id, new_shape);
		impl_->all_shapes.emplace_back(new_shape.get());
	}
	impl_->indexed_tetrahedrons.clear();
}

void Scene::AddTetrahedron(std::shared_ptr<SolverTetraeder> shape) {
	auto& tetra_vec = impl_->indexed_tetrahedrons[shape->id()];
	tetra_vec.emplace_back(std::move(shape));
}
}  // namespace renderer
