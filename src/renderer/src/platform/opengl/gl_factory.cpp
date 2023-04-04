#include "gl_factory.h"

#include <memory>

#include "log.h"
#include "platform/opengl/indexbuffer.h"
#include "platform/opengl/texture2d.h"
#include "platform/opengl/vertexarray.h"
#include "platform/opengl/vertexbuffer.h"
#include "renderer/texture2d.h"

namespace renderer::gl {
namespace {

GLenum ToGlFilterMode(FilterMode mode) {
	switch (mode) {
		case FilterMode::LINEAR:
			return GL_LINEAR;
		case FilterMode::NEAREST:
			return GL_NEAREST;
		default:
			assert(false && "Unknown FilterMode");
			return 0;
	}
}

GLenum ToGlWrapMode(WrapMode mode) {
	switch (mode) {
		case WrapMode::CLAMP_TO_EDGE:
			return GL_CLAMP_TO_EDGE;
		default:
			assert(false && "Unknown WrapMode");
			return 0;
	}
}

}  // namespace

std::unique_ptr<renderer::IndexBuffer> Factory::NewIndexBuffer(
		const uint32_t* data, const size_t size) {
	return std::make_unique<gl::IndexBuffer>(data, size);
}

std::unique_ptr<renderer::VertexArray> Factory::NewVertexArray() {
	return std::make_unique<gl::VertexArray>();
}

std::unique_ptr<renderer::VertexArray> Factory::NewVertexArray(
		std::vector<std::unique_ptr<renderer::VertexBuffer>>&& vbs,
		std::unique_ptr<renderer::IndexBuffer>&& ib) {
	return std::make_unique<gl::VertexArray>(std::move(vbs), std::move(ib));
}

// Initialization of dynamic buffer
std::unique_ptr<renderer::VertexBuffer> Factory::NewVertexBuffer(
		const size_t size) {
	return std::make_unique<gl::VertexBuffer>(size);
}

// Initialization of static buffer
std::unique_ptr<renderer::VertexBuffer> Factory::NewVertexBuffer(
		const void* data, const size_t size,
		std::unique_ptr<VertexBufferLayout>&& layout) {
	return std::make_unique<gl::VertexBuffer>(data, size, std::move(layout));
}

std::unique_ptr<renderer::Texture2D> Factory::NewTexture2D(
		const int width, const int height, const void* data, const int channels,
		const Format format, FilterMode filter, WrapMode wrap_mode,
		const bool generate_bitmap) {
	GLenum gl_filter    = ToGlFilterMode(filter);
	GLenum gl_wrap_mode = ToGlWrapMode(wrap_mode);
	return std::make_unique<gl::Texture2D>(width, height, data, channels, format,
	                                       gl_filter, gl_wrap_mode,
	                                       generate_bitmap);
}

}  // namespace renderer::gl
