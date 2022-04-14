#pragma once

#include <memory>
#include <vector>

#include "core.h"

class SceneShape;

class Scene {
 public:
	Scene();
	Scene(Scene&& other) noexcept;
	Scene& operator=(Scene&& other) noexcept;
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

 private:
	struct SceneImpl;
	void AddShape(const std::shared_ptr<BasicShape>& shape);

	// Idk why, but unique_ptr is not working
	SceneImpl* impl_;
};
