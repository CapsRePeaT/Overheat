#pragma once

#include <glm/vec4.hpp>
#include <memory>

#include "indexbuffer.h"
#include "vertexarray.h"

class RendererAPI {
 public:
	void Init();
	void SetViewPort(uint32_t x, uint32_t y, uint32_t w, uint32_t h);
	void SetClearColor(const glm::vec4& color);
	void Clear();
	void DrawIndexed(const VertexArray& va, const IndexBuffer& ib);
	void DrawIndexed(const VertexArray& va) {
		DrawIndexed(va, va.indexBuffer());
	}

	static RendererAPI instance() { return {}; }
};
