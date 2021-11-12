#pragma once

#include "renderer/renderer_api.h"

class OpenGLRendererAPI : public RendererAPI {
 public:
	void Init() final;
	void SetViewPort(uint32_t x, uint32_t y, uint32_t w, uint32_t h) final;
	void SetClearColor(const glm::vec4& color) final;
	void Clear() final;
	void DrawIndexed(const VertexArray& va, const IndexBuffer& ib) final;
};