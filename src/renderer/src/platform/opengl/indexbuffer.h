#pragma once

#include "renderer/indexbuffer.h"

class OpenGLIndexBuffer final : public IndexBuffer {
 public:
	OpenGLIndexBuffer(const uint32_t* data, const size_t count);
	template <size_t size>
	explicit OpenGLIndexBuffer(const std::array<uint32_t, size>& data)
			: IndexBuffer(data.data(), size) {}
	OpenGLIndexBuffer(OpenGLIndexBuffer&& other) noexcept;
	OpenGLIndexBuffer& operator=(OpenGLIndexBuffer&& other) noexcept;
	~OpenGLIndexBuffer() override;
	OpenGLIndexBuffer() = delete;
	OpenGLIndexBuffer(const OpenGLIndexBuffer&) = delete;
	OpenGLIndexBuffer& operator=(const OpenGLIndexBuffer&) = delete;
	void Bind() const final;
	void Unbind() const final;
	[[nodiscard]] uint32_t elements_count() const final { return count_; }

 private:
	uint32_t id_ = 0;
	uint32_t count_ = 0;
};
