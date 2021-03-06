#pragma once

#include <memory>
#include <optional>

#include "application/heatmap_material.h"
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

 private:
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

	struct Impl;
	std::unique_ptr<Impl> data_;
};

}  // namespace renderer
