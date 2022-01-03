#pragma once

#include "core.h"
#include "i_scene_viewport.h"

class GLSceneViewport final : public ISceneViewport {
 public:
	GLSceneViewport();
	~GLSceneViewport() override;
	void Initialize(int w, int h) override;
	void SetColorRange(Color min, Color max) override;
	void SetTemperatureRange(float min, float max) override;
	void RenderFrame() override;
	void Resize(int w, int h) override;
	void ClearResources() override;
	void ClearScene() override;

 protected:
	void AddShape(const std::shared_ptr<BasicShape>& shape) override;

 private:
	// Member and headers hiding because GLRenderer is public for other modules
	struct GLRendererData;

	// ClearResources must be invoked in dtor, but it's virtual. In this
	// particular case we can safely do it, but for consistency and future safety
	// I suggest to not to call virtual methods from dtors.
	// TODO: pin rule about invoking virtual methods from dtors in code style
	// documentation
	void ClearResourcesImpl();
	std::unique_ptr<GLRendererData> data_;
};
