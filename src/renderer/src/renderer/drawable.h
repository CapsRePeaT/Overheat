#pragma once

#include "renderer_context.h"

namespace renderer {

class Drawable {
 public:
	virtual bool SetContextForDraw(RendererContext& ctx) = 0;

	inline void SetIsVisible(bool is_visible) { is_visible_ = is_visible; }
	[[nodiscard]] inline bool is_visible() const { return is_visible_; }

 protected:
	bool is_visible_ = true;
};

}  // namespace renderer
