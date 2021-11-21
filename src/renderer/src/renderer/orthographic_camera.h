#pragma once

#include <glm/glm.hpp>

// TODO: Current way to handle zoom makes objects proportional to screen height

class OrthographicCamera {
 public:
	OrthographicCamera() = default;
	OrthographicCamera(float aspect_ratio, float zoom,
	                   std::pair<float, float> depth_bounds = {1.0f, 1.0f});
	OrthographicCamera(float left, float right, float bottom, float top,
	                   float near = -1.0f, float far = 1.0f);
	void SetPosition(glm::vec3 position) {
		position_ = position;
		RecalculateViewMatrix();
	}
	[[nodiscard]] glm::vec3 position() const { return position_; }
	void SetRotation(const float rotation) {
		rotation_ = rotation;
		RecalculateViewMatrix();
	}
	[[nodiscard]] float rotation() const { return rotation_; }
	void Zoom(const float delta) {
		zoom_level_ *= delta;
		RecalculateProjectionMatrix();
	}
	void SetAspectRatio(const float ratio) {
		aspect_ratio_ = ratio;
		RecalculateProjectionMatrix();
	}
	void SetProjection(float left, float right, float bottom, float top,
	                   float near = -1.0f, float far = 1.0f);
	[[nodiscard]] const glm::mat4& viewMatrix() const { return view_matrix_; }
	[[nodiscard]] const glm::mat4& projectionMatrix() const {
		return projection_matrix_;
	}
	[[nodiscard]] const glm::mat4& viewProjectionMatrix() const {
		return view_projection_matrix_;
	}
	[[nodiscard]] float zoom_level() const { return zoom_level_; }
	[[nodiscard]] float aspect_ratio() const { return aspect_ratio_; }

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
