#pragma once

namespace renderer {

enum class FilterMode { LINEAR, NEAREST };

enum class WrapMode { CLAMP_TO_EDGE };

enum class Format {
	FLOAT,
	UINT,
	UBYTE,
};

class Texture2D {
 public:
	virtual ~Texture2D() = default;

	virtual void Bind(int unit = -1) const = 0;
	virtual void Unbind(int unit) const = 0;
};

}  // namespace renderer
