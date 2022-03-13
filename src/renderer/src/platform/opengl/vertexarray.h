#pragma once

#include <vector>

#include "renderer/vertexarray.h"

namespace renderer {
namespace gl {

class VertexArray final : public renderer::VertexArray {
 public:
	VertexArray(std::unique_ptr<VertexBuffer>&& vb,
	            std::unique_ptr<IndexBuffer>&& ib);
	VertexArray() : VertexArray(nullptr, nullptr) {}
	VertexArray(VertexArray&& other) noexcept;
	VertexArray& operator=(VertexArray&& other) noexcept;
	~VertexArray() override;
	VertexArray(const VertexArray&) = delete;
	VertexArray& operator=(const VertexArray&) = delete;
	void SetBuffer(std::unique_ptr<VertexBuffer>&& vb) final;
	void SetIndexBuffer(std::unique_ptr<IndexBuffer>&& ib) final;
	[[nodiscard]] const IndexBuffer& indexBuffer() const final { return *ibo_; }
	void Bind() const final;
	void Unbind() const final;

 private:
	uint32_t id_;
	std::unique_ptr<VertexBuffer> vbo_;
	std::unique_ptr<IndexBuffer> ibo_;
};

}  // namespace gl
}  // namespace renderer
