#pragma once

#include <glm/glm.hpp>

// TODO: Current way to handle zoom makes objects proportional to screen height

class OrthographicCamera {
 public:
	OrthographicCamera() = default;
	OrthographicCamera(const float aspect_ratio, const float zoom,
	                   const std::pair<float, float> depth_bounds = {1.0f, 1.0f});
	OrthographicCamera(const float left, const float right, const float bottom,
	                   const float top, const float near = -1.0f,
	                   const float far = 1.0f);
	void SetPosition(const glm::vec3& position) {
		position_ = position;
		RecalculateViewMatrix();
	}
	void SetRotation(const float rotation) {
		rotation_ = rotation;
		RecalculateViewMatrix();
	}
	void Zoom(const float delta) {
		zoom_level_ *= delta;
		RecalculateProjectionMatrix();
	}
	void SetAspectRatio(const float ratio) {
		aspect_ratio_ = ratio;
		RecalculateProjectionMatrix();
	}
	void SetProjection(const float left, const float right, const float bottom,
	                   const float top, const float near = -1.0f,
	                   const float far = 1.0f);
	[[nodiscard]] const glm::mat4& viewMatrix() const { return view_matrix_; }
	[[nodiscard]] const glm::mat4& projectionMatrix() const {
		return projection_matrix_;
	}
	[[nodiscard]] const glm::mat4& viewProjectionMatrix() const {
		return view_projection_matrix_;
	}
	[[nodiscard]] float zoom_level() const { return zoom_level_; }
	[[nodiscard]] float aspect_ratio() const { return aspect_ratio_; }
	[[nodiscard]] glm::vec3 position() const { return position_; }
	[[nodiscard]] float rotation() const { return rotation_; }

 private:
	void RecalculateViewMatrix();
	void RecalculateProjectionMatrix();
	glm::mat4 view_matrix_ = glm::mat4(1.0f);
	glm::mat4 projection_matrix_ = glm::mat4(1.0f);
	glm::mat4 view_projection_matrix_ = glm::mat4(1.0f);
	glm::vec3 position_ = {0.0f, 0.0f, 0.0f};
	float rotation_ = 0.0f;
	float zoom_level_ = 1.0f;
	float aspect_ratio_ = 1.0f;
	float near_ = -1.0f;
	float far_ = 1.0f;
};
