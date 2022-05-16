#pragma once

#include <memory>
#include <vector>

#include "common.h"
#include "core.h"

namespace renderer {

class SceneShape;

class Scene {
 public:
	Scene();
	Scene(Scene&& other) noexcept = default;
	Scene& operator=(Scene&& other) noexcept = default;
	~Scene();
	Scene(const Scene&) = delete;
	Scene& operator=(Scene&) = delete;
	// Removes objects from scene
	void Clear();
	void AddShapes(const Core::Shapes& shapes) {
		for (const auto& shape : shapes) AddShape(shape);
	}
	void AddHeatmaps(const HeatmapStorage& heatmaps_storage);
	// Only for in-module usage (maybe will be removed and by-passed, TBT)
	[[nodiscard]] const std::vector<std::shared_ptr<SceneShape>>& shapes() const;
	[[nodiscard]] std::vector<std::shared_ptr<SceneShape>>& shapes();
	[[nodiscard]] const Heatmaps& heatmaps() const;
	[[nodiscard]] std::pair<float, float> bounds() const;
	[[nodiscard]] const std::shared_ptr<SceneShape>& shape_by_id(GlobalId id) const;
	[[nodiscard]] std::shared_ptr<SceneShape>& shape_by_id(GlobalId id);

 private:
	struct SceneImpl;
	void AddShape(const std::shared_ptr<BasicShape>& shape);

	// Idk why, but unique_ptr is not working
	std::unique_ptr<SceneImpl> impl_;
};

}  // namespace renderer
