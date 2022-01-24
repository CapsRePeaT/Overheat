#pragma once

#include <memory>

#include "application/heatmap_material.h"
#include "core.h"
#include "i_scene_viewport.h"
#include "renderer/orthographic_camera.h"

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
	void MoveCamera(int x, int y, int dX, int dY) override;
	void RotateCamera(int x, int y, int dX, int dY) override;
	void ZoomView(float delta) override;

 private:
	// ClearResources must be invoked in dtor, but it's virtual. In this
	// particular case we can safely do it, but for consistency and future safety
	// I suggest to not to call virtual methods from dtors.
	// TODO: pin rule about invoking virtual methods from dtors in code style
	// documentation
	void ClearResourcesImpl();
	std::unique_ptr<HeatmapMaterial> heatmap_material_;
	std::unique_ptr<OrthographicCamera> camera_;
	std::shared_ptr<Scene> scene_;
};
