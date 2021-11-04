#pragma once

#include "core.h"
#include "i_renderer.h"

class GLRenderer : public IRenderer {
 public:
	GLRenderer();
	~GLRenderer() override;
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
	// Member hiding because GLRenderer is public for other modules
	struct GLRendererData;
	// For calling in destructor
	void ClearResourcesImpl();
	std::unique_ptr<GLRendererData> data_;
};
