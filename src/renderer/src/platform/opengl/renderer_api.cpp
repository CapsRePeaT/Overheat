#include "renderer_api.h"

#include <glad/glad.h>
#include <spdlog/spdlog.h>

#include <string_view>

#include "renderer/renderer_api.h"

namespace renderer {
namespace {

inline decltype(GL_TRIANGLES) PrimitiveTypeToGL(const PrimitiveType draw_as) {
	switch (draw_as) {
		case PrimitiveType::POINTS:
			return GL_POINTS;
		case PrimitiveType::LINES:
			return GL_LINES;
		case PrimitiveType::LINE_STRIP:
			return GL_LINE_STRIP;
		case PrimitiveType::LINE_LOOP:
			return GL_LINE_LOOP;
		case PrimitiveType::TRIANGLES:
			return GL_TRIANGLES;
		case PrimitiveType::TRIANGLE_STRIP:
			return GL_TRIANGLE_STRIP;
		case PrimitiveType::TRIANGLE_FAN:
			return GL_TRIANGLE_FAN;
		default:
			assert(false && "Unkown primitive type");
	}
}

// Signature from
// https://www.khronos.org/opengl/wiki/Debug_Output#Getting_messages
void OpenGLMessageCallback(GLenum source, GLenum type, GLuint id,
                           GLenum severity, GLsizei length,
                           const GLchar* message, const void* user_param) {
	switch (severity) {
		case GL_DEBUG_SEVERITY_HIGH:
			spdlog::critical(message);
			return;
		case GL_DEBUG_SEVERITY_MEDIUM:
			spdlog::error(message);
			return;
		case GL_DEBUG_SEVERITY_LOW:
			spdlog::warn(message);
			return;
		case GL_DEBUG_SEVERITY_NOTIFICATION:
			spdlog::trace(message);
			return;
		default:
			assert(false && "Unknown severity level!");
	}
}

}  // namespace

namespace gl {

void RendererAPI::InitImpl() {
#ifndef NDEBUG
	spdlog::info("Setting debug opengl context..");
	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageCallback(OpenGLMessageCallback, nullptr);
	glDebugMessageControl(/*source=*/GL_DONT_CARE, /*type=*/GL_DONT_CARE,
	                      GL_DEBUG_SEVERITY_NOTIFICATION, /*ids count=*/0,
	                      /*ids=*/nullptr, /*enabled=*/GL_FALSE);
#endif
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_DEPTH_TEST);
}

void RendererAPI::SetViewPort(const uint32_t x, const uint32_t y,
                              const uint32_t w, const uint32_t h) {
	glViewport(x, y, w, h);  // NOLINT(cppcoreguidelines-narrowing-conversions)
}

void RendererAPI::SetClearColor(const glm::vec4 color) {
	glClearColor(color.r, color.g, color.b, color.a);
}

void RendererAPI::Clear() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void RendererAPI::DrawIndexedImpl(const VertexArray& va, const IndexBuffer& ib,
                                  const PrimitiveType draw_as) const {
	va.Bind();
	glDrawElements(
			PrimitiveTypeToGL(draw_as), ib.elements_count(),
			GL_UNSIGNED_INT,  // NOLINT(cppcoreguidelines-narrowing-conversions)
			nullptr);
}

}  // namespace gl
}  // namespace renderer
