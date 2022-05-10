#pragma once

namespace renderer {

enum class FilterMode { LINEAR, NEAREST };

enum class WrapMode { CLAMP_TO_EDGE };

class Texture2D {
 public:
	virtual ~Texture2D() = default;

	virtual void Bind(int unit = -1) const = 0;
};

}  // namespace renderer
