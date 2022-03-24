#pragma once

#include "core.h"
#include "i_renderer.h"

class GLRenderer final : public IRenderer {
 public:
	GLRenderer();
	~GLRenderer() override;
	void Initialize(const int w, const int h) override;
	void SetColorRange(const Color min, const Color max) override;
	void SetTemperatureRange(const float min, const float max) override;
	void RenderFrame() override;
	void Resize(const int w, const int h) override;
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