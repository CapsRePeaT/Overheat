#pragma once

#include <glm/vec4.hpp>

#include <memory>

#include "indexbuffer.h"
#include "vertexarray.h"

enum class PrimitiveType {
	POINTS,
	LINES,
	LINE_STRIP,
	LINE_LOOP,
	TRIANGLES,
	TRIANGLE_STRIP,
	TRIANGLE_FAN
};

class RendererAPI {
 public:
	enum class API { None = 0, OpenGL = 1 };
	virtual ~RendererAPI() = default;
	virtual void Init() = 0;
	virtual void SetViewPort(uint32_t x, uint32_t y, uint32_t w, uint32_t h) = 0;
	virtual void SetClearColor(glm::vec4 color) = 0;
	virtual void Clear() = 0;

	inline void DrawIndexed(const VertexArray& va, const IndexBuffer& ib,
	                 PrimitiveType draw_as = PrimitiveType::TRIANGLES) {
		DrawIndexedImpl(va, ib, draw_as);
	}
	inline void DrawIndexed(const VertexArray& va,
	                 PrimitiveType draw_as = PrimitiveType::TRIANGLES) {
		DrawIndexedImpl(va, va.indexBuffer(), draw_as);
	}
	[[nodiscard]] static std::unique_ptr<RendererAPI> instance();
	[[nodiscard]] static API api() { return api_; }

 protected:
	virtual void DrawIndexedImpl(const VertexArray& va, const IndexBuffer& ib,
	                             PrimitiveType draw_as) = 0;

 private:
	static API api_;
};
