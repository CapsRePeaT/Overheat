#include "gl_renderer.h"

#include <glad/glad.h>
#include <spdlog/spdlog.h>

void GLRenderer::Initialize(int w, int h) {
	if (!gladLoadGL()) spdlog::error("Failed to initialize opengl functions");

	spdlog::debug("GLAD initialized");
	glClearColor(1, 1, 1, 1);
}

void GLRenderer::SetDrawMode(const DrawMode) {}

void GLRenderer::RenderFrame() {
	glClear(GL_COLOR_BUFFER_BIT);

}

void GLRenderer::Resize(int w, int h) {}

void GLRenderer::Clear() {

}

GLRenderer::~GLRenderer() {}
