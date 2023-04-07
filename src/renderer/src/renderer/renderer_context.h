#pragma once

#include <functional>

#include "i_camera.h"
#include "i_renderer_factory.h"
#include "vertexarray.h"

namespace renderer {

// TODO: store renderer commands here. Make Draw() to make new renderer command;
class RendererContext {
 public:
	explicit RendererContext(const ICamera& camera) : camera_(&camera) {}

	[[nodiscard]] const ICamera& camera() { return *camera_; }
	void SetVao(const VertexArray& vao) { vao_ = &vao; }
	[[nodiscard]] const VertexArray& vao() { return *vao_; }
	void SetMaterialCallbacks(std::function<void()> use_cb,
	                          std::function<void()> unuse_cb) {
		material_init_cb_  = std::move(use_cb);
		material_unuse_cb_ = std::move(unuse_cb);
	}
	void UseMaterial() { material_init_cb_(); }
	void UnuseMaterial() { material_unuse_cb_(); }

 private:
	// Non-ownership ptrs (object is not intendend to be stored anywhere)
	const ICamera* camera_;
	const VertexArray* vao_;
	std::function<void()> material_init_cb_;
	std::function<void()> material_unuse_cb_;
};

}  // namespace renderer