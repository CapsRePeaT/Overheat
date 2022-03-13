#pragma once

#include <glm/vec4.hpp>

#include <memory>

#include "i_scene_viewport.h"
#include "indexbuffer.h"
#include "renderer/i_renderer_factory.h"
#include "vertexarray.h"

namespace renderer {

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
	virtual ~RendererAPI() = default;

	static RendererAPI& Init(API api);

	virtual void SetViewPort(uint32_t x, uint32_t y, uint32_t w, uint32_t h) = 0;
	virtual void SetClearColor(glm::vec4 color) = 0;
	virtual void Clear() = 0;

	inline void DrawIndexed(
			const VertexArray& va, const IndexBuffer& ib,
			PrimitiveType draw_as = PrimitiveType::TRIANGLES) const {
		DrawIndexedImpl(va, ib, draw_as);
	}
	inline void DrawIndexed(
			const VertexArray& va,
			PrimitiveType draw_as = PrimitiveType::TRIANGLES) const {
		DrawIndexedImpl(va, va.indexBuffer(), draw_as);
	}
	[[nodiscard]] static RendererAPI& instance();
	[[nodiscard]] static API api();
	[[nodiscard]] static IRendererFactory& factory();

 protected:
	virtual void DrawIndexedImpl(const VertexArray& va, const IndexBuffer& ib,
	                             PrimitiveType draw_as) const = 0;
	virtual void InitImpl() = 0;

 private:
	static std::unique_ptr<RendererAPI> createInstance();
};

}  // namespace renderer
