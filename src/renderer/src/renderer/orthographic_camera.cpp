#include "orthographic_camera.h"

#include <spdlog/spdlog.h>

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>

OrthographicCamera::OrthographicCamera(float aspect_ratio, float zoom,
                                       std::pair<float, float> depth_bounds)
		: projection_matrix_(glm::ortho(-aspect_ratio * zoom, aspect_ratio * zoom,
                                    -zoom, zoom, depth_bounds.first,
                                    depth_bounds.second)),
			aspect_ratio_(aspect_ratio),
			zoom_level_(zoom),
			near_(depth_bounds.first),
			far_(depth_bounds.second) {}
OrthographicCamera::OrthographicCamera(float left, float right, float bottom,
                                       float top, float near, float far)
		: projection_matrix_(glm::ortho(left, right, bottom, top, near, far)),
			near_(near),
			far_(far) {
	view_projection_matrix_ = projection_matrix_ * view_matrix_;
}
void OrthographicCamera::SetProjection(float left, float right, float bottom,
                                       float top, float near, float far) {
	projection_matrix_ = glm::ortho(left, right, bottom, top, near, far);
	view_projection_matrix_ = projection_matrix_ * view_matrix_;
	near_ = near;
	far_ = far;
}
void OrthographicCamera::RecalculateViewMatrix() {
	glm::mat4 transform =
			glm::translate(glm::mat4(1.0f), position_) *
			glm::rotate(glm::mat4(1.0f), glm::radians(rotation_), glm::vec3(0, 0, 1));

	view_matrix_ = glm::inverse(transform);
	view_projection_matrix_ = projection_matrix_ * view_matrix_;
}
void OrthographicCamera::RecalculateProjectionMatrix() {
	SetProjection(-aspect_ratio_ * zoom_level_, aspect_ratio_ * zoom_level_,
	              -zoom_level_, zoom_level_, near_, far_);
}
