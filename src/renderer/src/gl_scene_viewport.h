#pragma once

#include <memory>
#include <optional>

#include "application/heatmap_material.h"
#include "application/temperature_bar.h"
#include "application/temperature_bar_material.h"
#include "renderer/debug/debug_heatmap_material.h"
#include "camera_controller.h"
#include "core.h"
#include "i_scene_viewport.h"

namespace renderer {

class GLSceneViewport final : public ISceneViewport {
 public:
	explicit GLSceneViewport(std::shared_ptr<Scene> scene);
	~GLSceneViewport() override;
	void Initialize(int w, int h) override;
	void SetColorRange(Color min, Color max) override;
	void SetTemperatureRange(float min, float max) override;
	void RenderFrame() override;
	void ClearResources() override;
	void Resize(int w, int h) override;
	void MoveCamera(Vec2D screenPoint, Vec2D delta) override;
	void RotateCamera(Vec2D screenPoint, Vec2D delta) override;
	void ZoomView(float delta) override;
	void SetVisibility(const GlobalIds& to_change, bool is_visible) override;
	void SetDrawMode(DrawMode mode) override;
	void SetStratifiedStep(float step) override;
	void ClearSelection() override;
	void SetSelection(const GlobalIds& to_change, HighlightType type) override;

 private:
 	void InitHeatmapMaterials();
	// ClearResources must be invoked in dtor, but it's virtual. In this
	// particular case we can safely do it, but for consistency and future safety
	// I suggest to not to call virtual methods from dtors.
	// TODO: pin rule about invoking virtual methods from dtors in code style
	// documentation
	void ClearResourcesImpl();
	void OpenGlInit(int w, int h);
	void ApplicationInit(int w, int h);
	void DebugInit(int w, int h);
	std::optional<std::vector<HeatmapMaterial>> heatmap_materials_;
	std::unique_ptr<SphericalCameraController> camera_controller_;
	std::shared_ptr<Scene> scene_;
	std::unique_ptr<TemperatureBarMaterial> temperature_bar_material_;
	std::unique_ptr<TemperatureBar> temperature_bar_;


	struct Impl;
	std::unique_ptr<Impl> data_;
};

}  // namespace renderer
