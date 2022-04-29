#pragma once

#include "renderer/renderer_api.h"

namespace renderer::gl {

class RendererAPI final : public renderer::RendererAPI {
 public:
	void SetViewPort(uint32_t x, uint32_t y, uint32_t w, uint32_t h) final;
	void SetClearColor(glm::vec4 color) final;
	void Clear() final;
	void DrawIndexedImpl(const VertexArray& va, const IndexBuffer& ib,
	                     PrimitiveType draw_as) const final;
 protected:
	void InitImpl() final;
};

}  // namespace renderer::gl
