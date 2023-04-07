#pragma once

#include "renderer_context.h"

namespace renderer {

class Drawable {
 public:
	virtual bool SetContextForDraw(RendererContext& ctx) = 0;
};

}  // namespace renderer
