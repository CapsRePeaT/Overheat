#pragma once

#include <array>
#include <cstddef>
#include <memory>

class IndexBuffer {
 public:
	virtual ~IndexBuffer() = default;
	virtual void Bind() const = 0;
	virtual void Unbind() const = 0;
	[[nodiscard]] virtual uint32_t elements_count() const = 0;
	static std::unique_ptr<IndexBuffer> Create(const uint32_t* data, size_t size);
	template <size_t size>
	static std::unique_ptr<IndexBuffer> Create(
			const std::array<uint32_t, size>& data) {
		return Create(data.data(), size);
	}
};
