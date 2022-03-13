#pragma once

#include <array>
#include <cstdint>

namespace renderer {

class IndexBuffer {
 public:
	virtual ~IndexBuffer() = default;
	virtual void Bind() const = 0;
	virtual void Unbind() const = 0;
	[[nodiscard]] virtual uint32_t elements_count() const = 0;
};

}  // namespace renderer
