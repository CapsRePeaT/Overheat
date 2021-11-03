#include "renderer_api.h"

#include <glad/glad.h>
#include <spdlog/spdlog.h>

void OpenGLMessageCallback(unsigned source, unsigned type, unsigned id,
                           unsigned severity, int length, const char* message,
                           const void* user_param) {
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

void RendererAPI::Init() {
#ifndef NDEBUG
	spdlog::info("Setting debug opengl context..");
	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageCallback(OpenGLMessageCallback, nullptr);
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE,
	                      GL_DEBUG_SEVERITY_NOTIFICATION, 0, nullptr, GL_FALSE);
#endif
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_DEPTH_TEST);
}
void RendererAPI::SetViewPort(uint32_t x, uint32_t y, uint32_t w, uint32_t h) {
	glViewport(x, y, w, h);  // NOLINT(cppcoreguidelines-narrowing-conversions)
}
void RendererAPI::SetClearColor(const glm::vec4& color) {
	glClearColor(color.r, color.g, color.b, color.a);
}
void RendererAPI::Clear() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
void RendererAPI::DrawIndexed(const VertexArray& va, const IndexBuffer& ib) {
	va.Bind();
	glDrawElements(GL_TRIANGLES, ib.elementsCount(), GL_UNSIGNED_INT,
	               nullptr);  // NOLINT(cppcoreguidelines-narrowing-conversions)
}
