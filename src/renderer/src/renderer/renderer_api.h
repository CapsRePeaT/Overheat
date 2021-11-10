#pragma once

#include <memory>

#include <glm/vec4.hpp>

#include "indexbuffer.h"
#include "vertexarray.h"

class RendererAPI {
 public:
	enum class API { None = 0, OpenGL = 1 };
	
	virtual ~RendererAPI() = default;

	virtual void Init() = 0;
	virtual void SetViewPort(uint32_t x, uint32_t y, uint32_t w, uint32_t h) = 0;
	virtual void SetClearColor(const glm::vec4& color) = 0;
	virtual void Clear() = 0;
	virtual void DrawIndexed(const VertexArray& va, const IndexBuffer& ib) = 0;
	void DrawIndexed(const VertexArray& va) {
		DrawIndexed(va, va.indexBuffer());
	}

	static std::unique_ptr<RendererAPI> instance();
	static API api() { return api_; }

	private:
		static API api_;
};
