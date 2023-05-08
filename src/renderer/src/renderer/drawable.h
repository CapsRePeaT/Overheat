#pragma once

#include "common.h"
#include "renderer_context.h"

namespace renderer {

class Drawable {
 public:
	virtual bool SetContextForDraw(RendererContext& ctx) = 0;
	virtual bool SetContextForDrawWireframe(RendererContext& ctx) { return false; };

	inline void SetIsVisible(bool is_visible) { is_visible_ = is_visible; }
	[[nodiscard]] inline bool is_visible() const { return is_visible_; }
	inline void SetHighlightType(HighlightType highlight_type) {
		highlight_type_ = highlight_type;
	}
	[[nodiscard]] inline HighlightType highlight_type() const {
		return highlight_type_;
	}

 protected:
	bool is_visible_ = true;
	HighlightType highlight_type_ = HighlightType::None;
};

inline glm::vec3 HighlightTypeToColor(HighlightType highlight_type) {
	switch (highlight_type) {
		case HighlightType::None:
			return consts::color_not_selected;
		case HighlightType::Selected:
			return consts::color_selected;
		case HighlightType::ActiveSelected:
			return consts::color_active_selected;
		default:
			assert(false && "Unkown highlight type!");
			return consts::vec3_0;
	}
}

}  // namespace renderer
