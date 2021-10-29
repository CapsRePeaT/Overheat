#pragma once

#include <glad/glad.h>

#include <vector>

class VertexBufferLayout {
 public:
	struct VertexBufferElement;
	using VertexBufferElements = std::vector<VertexBufferElement>;

	struct VertexBufferElement  // NOLINT(cppcoreguidelines-avoid-magic-numbers)
	{
		int count;
		GLenum type;
		GLboolean normalized;

		[[nodiscard]] unsigned int size() const;
	};

	VertexBufferLayout() = default;
	~VertexBufferLayout() = default;

	VertexBufferLayout(VertexBufferLayout&&) = default;
	VertexBufferLayout& operator=(VertexBufferLayout&&) = default;

	VertexBufferLayout(const VertexBufferLayout&) = delete;
	VertexBufferLayout& operator=(const VertexBufferLayout&) = delete;

	template <typename T>
	void Push(unsigned int count) {
		elements_.push_back({count, TypeToGlEnum<T>(), sizeof(T) == 1});
		stride_ += sizeof(T) * count;
	}
	[[nodiscard]] inline int32_t stride() const { return stride_; }

	[[nodiscard]] inline const VertexBufferElements& elements() const {
		return elements_;
	}

 private:
	template <typename T>
	static constexpr GLenum TypeToGlEnum() {
		if constexpr (std::is_same_v<T, float>)
			return GL_FLOAT;
		else if constexpr (std::is_same_v<T, uint32_t>)
			return GL_UNSIGNED_INT;
		else if constexpr (std::is_same_v<T, int8_t>)
			return GL_BYTE;
	}

	int32_t stride_ = 0;
	VertexBufferElements elements_;
};
