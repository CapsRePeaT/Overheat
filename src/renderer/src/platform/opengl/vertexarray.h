#pragma once

#include <vector>

#include "renderer/vertexarray.h"

namespace renderer::gl {

class VertexArray final : public renderer::VertexArray {
 public:
	VertexArray(std::vector<std::shared_ptr<renderer::VertexBuffer>>&& vb,
	            std::unique_ptr<renderer::IndexBuffer>&& ib);
	VertexArray() : VertexArray({}, nullptr) {}
	VertexArray(VertexArray&& other) noexcept;
	VertexArray& operator=(VertexArray&& other) noexcept;
	~VertexArray() override;
	VertexArray(const VertexArray&) = delete;
	VertexArray& operator=(const VertexArray&) = delete;
	void SetBuffers(std::vector<std::shared_ptr<renderer::VertexBuffer>>&& vbs) final;
	void SetIndexBuffer(std::unique_ptr<renderer::IndexBuffer>&& ib) final;
	[[nodiscard]] const renderer::IndexBuffer& indexBuffer() const final {
		return *ibo_;
	}
	void Bind() const final;
	void Unbind() const final;

 private:
	uint32_t id_;
	std::vector<std::shared_ptr<renderer::VertexBuffer>> vbos_;
	std::unique_ptr<renderer::IndexBuffer> ibo_;
};

}  // namespace renderer::gl
