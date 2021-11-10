#ifndef OVERHEAT_SRC_RENDERER_SRC_OPENGL_INDEXBUFFER_H_
#define OVERHEAT_SRC_RENDERER_SRC_OPENGL_INDEXBUFFER_H_

#include <glad/glad.h>

#include <array>
#include <cstddef>

class IndexBuffer {
 public:
	IndexBuffer(const uint32_t* data, size_t count);
	template <size_t size>
	explicit IndexBuffer(const std::array<uint32_t, size>& data)
			: IndexBuffer(data.data(), size) {}

	~IndexBuffer();

	IndexBuffer(IndexBuffer&& other) noexcept;
	IndexBuffer& operator=(IndexBuffer&& other) noexcept;

	IndexBuffer() = delete;
	IndexBuffer(const IndexBuffer&) = delete;
	IndexBuffer& operator=(const IndexBuffer&) = delete;

	void Bind() const;
	static void Unbind();

	[[nodiscard]] uint32_t elementsCount() const { return count_; };

 private:
	uint32_t id_ = 0;
	uint32_t count_ = 0;
};

#endif  // OVERHEAT_SRC_RENDERER_SRC_OPENGL_INDEXBUFFER_H_
