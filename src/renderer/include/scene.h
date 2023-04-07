#pragma once

#include <memory>
#include <vector>

#include "common.h"
#include "core.h"

namespace renderer {

class BoxShape;

class Scene {
 public:
	Scene();
	Scene(Scene&& other) noexcept            = default;
	Scene& operator=(Scene&& other) noexcept = default;
	~Scene();
	Scene(const Scene&)      = delete;
	Scene& operator=(Scene&) = delete;
	// Removes objects from scene
	void Clear();
	void AddFileRepresentation(FileRepresentation& file_representation,
	                           bool use_layered_heatmaps);
	void AddShapes(const Core::Shapes& shapes) {
		for (const auto& shape : shapes) AddShape(shape);
	}
	void AddHeatmaps(const HeatmapStorage& heatmaps_storage);
	bool UpdateForRenderer();
	// Only for in-module usage (maybe will be removed and by-passed, TBT)
	[[nodiscard]] const std::vector<std::shared_ptr<BoxShape>>& shapes() const;
	[[nodiscard]] std::vector<std::shared_ptr<BoxShape>>& shapes();
	[[nodiscard]] const Heatmaps& heatmaps() const;
	[[nodiscard]] std::pair<float, float> bounds() const;
	[[nodiscard]] const std::shared_ptr<BoxShape>& shape_by_id(GlobalId id) const;
	[[nodiscard]] std::shared_ptr<BoxShape>& shape_by_id(GlobalId id);
	
	// only for viewport
	void SetTemperatureRange(float min, float max);
	void SetColorRange(std::array<float, 3> min, std::array<float, 3> max);
	void SetDrawMode(DrawMode mode);
	void SetStratifiedStep(float step);

 private:
	struct SceneImpl;
	void AddShape(const std::shared_ptr<BasicShape>& shape);
	void InitHeatmapMaterials();

	std::unique_ptr<SceneImpl> impl_;
	bool use_layered_heatmaps_ = true;
};

}  // namespace renderer
