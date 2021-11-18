#pragma once

#include "renderer/renderer_api.h"

class OpenGLRendererAPI final : public RendererAPI {
 public:
	void Init() final;
	void SetViewPort(const uint32_t x, const uint32_t y, const uint32_t w,
	                 const uint32_t h) final;
	void SetClearColor(const glm::vec4 color) final;
	void Clear() final;
	void DrawIndexed(const VertexArray& va, const IndexBuffer& ib) final;
};
