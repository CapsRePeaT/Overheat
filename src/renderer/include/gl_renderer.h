#pragma once
#include "i_renderer.h"

#include <glm/glm.hpp>


class GLRenderer : public IRenderer {
 public:
	~GLRenderer() override;
	void Initialize(int w = 0, int h = 0) override;
	void SetDrawMode(const DrawMode mode) override;
	void RenderFrame() override;
	void Resize(int w, int h) override;
	void Clear() override;

 private:

};
