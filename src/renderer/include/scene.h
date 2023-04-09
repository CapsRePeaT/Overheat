#pragma once

#include <memory>
#include <vector>

#include "glm/glm.hpp"

#include "../../fem_solver/src/solver_shapes.hpp"
#include "common.h"
#include "core.h"

namespace renderer {

class BoxShape;
class Drawable;

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
	[[nodiscard]] const std::vector<Drawable*>& shapes() const;
	[[nodiscard]] std::pair<float, float> bounds() const;
	[[nodiscard]] const std::shared_ptr<BoxShape>& shape_by_id(GlobalId id) const;
	[[nodiscard]] std::shared_ptr<BoxShape>& shape_by_id(GlobalId id);

	// only for viewport
	void SetTemperatureRange(float min, float max);
	void SetColors(const std::array<glm::vec3, 5>& colors);
	void SetDrawMode(DrawMode mode);
	void SetStratifiedStep(float step);
	void SetVisibility(const GlobalIds& to_change, bool is_visible);

 private:
	struct SceneImpl;
	void AddShape(const std::shared_ptr<BasicShape>& shape);
	void InitHeatmapMaterials();
	void AddTetrahedrons(
			const std::vector<std::shared_ptr<SolverShape>>& core_shapes) {
		for (const auto& shape : core_shapes) {
			auto casted_shape = std::dynamic_pointer_cast<SolverTetraeder>(shape);
			if (casted_shape)
				AddTetrahedron(casted_shape);
		}
	};
	void AddTetrahedron(std::shared_ptr<SolverTetraeder> shape);

	std::unique_ptr<SceneImpl> impl_;
	bool use_layered_heatmaps_ = true;
};

}  // namespace renderer
