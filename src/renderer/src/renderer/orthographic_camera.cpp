#include "orthographic_camera.h"

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/quaternion_geometric.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "constants.h"
#include "renderer/scene_object.h"

OrthographicCamera::OrthographicCamera(
		const float aspect_ratio, const float zoom,
		const std::pair<float, float> depth_bounds)
		: projection_matrix_(glm::ortho(-aspect_ratio * zoom, aspect_ratio * zoom,
                                    -zoom, zoom, depth_bounds.first,
                                    depth_bounds.second)),
			zoom_level_(zoom),
			aspect_ratio_(aspect_ratio),
			near_(depth_bounds.first),
			far_(depth_bounds.second) {
	CalculateViewProjectionMatrix();
}

OrthographicCamera::OrthographicCamera(const float left, const float right,
                                       const float bottom, const float top,
                                       const float near, const float far)
		: projection_matrix_(glm::ortho(left, right, bottom, top, near, far)),
			near_(near),
			far_(far) {
	CalculateViewProjectionMatrix();
}

void OrthographicCamera::SetProjection(const float left, const float right,
                                       const float bottom, const float top,
                                       const float near, const float far) {
	projection_matrix_ = glm::ortho(left, right, bottom, top, near, far);
	CalculateViewProjectionMatrix();
	near_ = near;
	far_ = far;
}

void OrthographicCamera::RecalculateViewMatrix() {
	view_matrix_ = glm::inverse(transform());
	CalculateViewProjectionMatrix();
}

void OrthographicCamera::RecalculateProjectionMatrix() {
	SetProjection(-aspect_ratio_ * zoom_level_, aspect_ratio_ * zoom_level_,
	              -zoom_level_, zoom_level_, near_, far_);
}

void OrthographicCamera::CalculateViewProjectionMatrix() {
	view_projection_matrix_ = projection_matrix_ * view_matrix_;
}

void OrthographicCamera::LookAt(const glm::vec3 point_to_view) {
	auto normalizedDirection = glm::normalize(point_to_view - position());
	SetRotation(glm::quatLookAt(normalizedDirection, consts::vec3_z));
}

void OrthographicCamera::SetPosition(const glm::vec3 position) {
	SceneObject::SetPosition(position);
	RecalculateViewMatrix();
}

void OrthographicCamera::SetRotation(const glm::quat rotation) {
	SceneObject::SetRotation(rotation);
	RecalculateViewMatrix();
}

void OrthographicCamera::Zoom(const float delta) {
	zoom_level_ *= delta;
	RecalculateProjectionMatrix();
}

void OrthographicCamera::SetAspectRatio(const float ratio) {
	aspect_ratio_ = ratio;
	RecalculateProjectionMatrix();
}
