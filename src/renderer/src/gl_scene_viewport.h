#pragma once

#include <memory>
#include <optional>

#include "application/heatmap_material.h"
#include "application/wireframe_material.h"
#include "application/temperature_bar.h"
#include "application/temperature_bar_material.h"
#include "camera_controller.h"
#include "core.h"
#include "i_scene_viewport.h"
#include "renderer/debug/debug_heatmap_material.h"
#include "text/font.h"
#include "text/text2d.h"
#include "renderer/drawable.h"


namespace renderer {

class GLSceneViewport final : public ISceneViewport {
 public:
	explicit GLSceneViewport(std::shared_ptr<Scene> scene);
	~GLSceneViewport() override;
	void Initialize(int w, int h) override;
	void SetColors(const Colors& colors) override;
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
	// ClearResources must be invoked in dtor, but it's virtual. In this
	// particular case we can safely do it, but for consistency and future safety
	// I suggest to not to call virtual methods from dtors.
	// TODO: pin rule about invoking virtual methods from dtors in code style
	// documentation
	void ClearResourcesImpl();
	void OpenGlInit(int w, int h);
	void ApplicationInit(int w, int h);
	void DebugInit(int w, int h);
	void InitTemperatureBar(float min_temp = 0, float max_temp = 0);
	void Draw(RendererContext& ctx, Drawable& drawable);

	glm::ivec2 view_size_ = {0, 0};
	std::unique_ptr<SphericalCameraController> camera_controller_;
	std::shared_ptr<Scene> scene_;
	std::unique_ptr<TemperatureBar> temperature_bar_;
	float tbar_thickness_ = 25.0f;
	glm::vec2 tbar_screen_margins_ = {20.0f, 50.0f};
	std::shared_ptr<Font> font_;
	std::unique_ptr<WireframeMaterial> wireframe_material_;

	struct Impl;
	std::unique_ptr<Impl> data_;
};

}  // namespace renderer
