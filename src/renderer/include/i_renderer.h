#pragma once

#include <functional>

#include "common.h"

class IRenderer {
 public:
	virtual ~IRenderer() = default;
	// Initializes OpenGL functions and resources.
	// Needs to be called with active OpenGL context.
  virtual void Initialize(int w = 0, int h = 0) = 0;
  virtual void RenderFrame() = 0;
	virtual void Resize(int w, int h) = 0;
	virtual void Clear() = 0;
  virtual void SetDrawMode(const DrawMode) = 0;
 private:
};


