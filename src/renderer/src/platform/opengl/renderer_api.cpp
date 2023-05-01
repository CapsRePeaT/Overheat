#include "renderer_api.h"

#include <glad/glad.h>

#include <string_view>

#include "log.h"
#include "renderer/renderer_api.h"

namespace renderer::gl {
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
			return {};
	}
}

// Signature from
// https://www.khronos.org/opengl/wiki/Debug_Output#Getting_messages
void OpenGLMessageCallback(GLenum  /*source*/, GLenum  /*type*/, GLuint  /*id*/,
                           GLenum severity, GLsizei  /*length*/,
                           const GLchar* message, const void*  /*user_param*/) {
	switch (severity) {
		case GL_DEBUG_SEVERITY_HIGH:
			LOG_CRITICAL(message);
			return;
		case GL_DEBUG_SEVERITY_MEDIUM:
			LOG_ERROR(message);
			return;
		case GL_DEBUG_SEVERITY_LOW:
			LOG_WARN(message);
			return;
		case GL_DEBUG_SEVERITY_NOTIFICATION:
			LOG_TRACE(message);
			return;
		default:
			assert(false && "Unknown severity level!");
	}
}

}  // namespace

void RendererAPI::InitImpl() {
#ifndef NDEBUG
	LOG_INFO("Setting debug opengl context..");
	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageCallback(OpenGLMessageCallback, nullptr);
	glDebugMessageControl(/*source=*/GL_DONT_CARE, /*type=*/GL_DONT_CARE,
	                      GL_DEBUG_SEVERITY_NOTIFICATION, /*ids count=*/0,
	                      /*ids=*/nullptr, /*enabled=*/GL_FALSE);
#endif
	// FIXME: seems like it's does not work
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_PRIMITIVE_RESTART_FIXED_INDEX);
	glEnable(GL_POLYGON_OFFSET_LINE);
	glEnable(GL_POLYGON_OFFSET_FILL);
	glEnable(GL_LINE_SMOOTH);
	// glLineWidth(2.0f);
	float lineWidth[2];
	glGetFloatv(GL_LINE_WIDTH_RANGE, lineWidth);
	LOG_TRACE("Line width range from {} to {}", lineWidth[0], lineWidth[1]);
	glEnable(GL_MULTISAMPLE);
}

void RendererAPI::SetViewPort(const uint32_t x, const uint32_t y,
                              const uint32_t w, const uint32_t h) {
	glViewport(x, y, w, h);  // NOLINT(bugprone-narrowing-conversions)
}

void RendererAPI::SetClearColor(const glm::vec4 color) {
	glClearColor(color.r, color.g, color.b, color.a);
}

void RendererAPI::EnableDepthBuffer(bool value) {
	if (value) {
		glEnable(GL_DEPTH_TEST);
	}
	else {
		glDisable(GL_DEPTH_TEST);
	}
}

void RendererAPI::Clear() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void RendererAPI::DrawIndexedImpl(const VertexArray& va, const IndexBuffer& ib,
                                  const PrimitiveType draw_as) const {
	va.Bind();
	glDrawElements( 
			PrimitiveTypeToGL(draw_as), ib.elements_count(),  // NOLINT(bugprone-narrowing-conversions)
			GL_UNSIGNED_INT,
			nullptr);
	va.Unbind();
}

}  // namespace renderer::gl
