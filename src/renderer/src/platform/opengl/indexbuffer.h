#pragma once

#include <cstddef>

#include "renderer/indexbuffer.h"

namespace renderer::gl {

class IndexBuffer final : public renderer::IndexBuffer {
 public:
	IndexBuffer(const uint32_t* data, size_t count);
	template <size_t size>
	explicit IndexBuffer(const std::array<uint32_t, size>& data)
			: IndexBuffer(data.data(), size) {}
	IndexBuffer(IndexBuffer&& other) noexcept;
	IndexBuffer& operator=(IndexBuffer&& other) noexcept;
	~IndexBuffer() override;
	IndexBuffer()                   = delete;
	IndexBuffer(const IndexBuffer&) = delete;
	IndexBuffer& operator=(const IndexBuffer&) = delete;
	void Bind() const final;
	void Unbind() const final;
	[[nodiscard]] uint32_t elements_count() const final { return count_; }

 private:
	uint32_t id_    = 0;
	uint32_t count_ = 0;
};

}  // namespace renderer::gl
