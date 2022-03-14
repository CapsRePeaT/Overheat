#pragma once

#include <glm/glm.hpp>

#include "constants.h"
#include "i_camera.h"
#include "platform/opengl/vertexarray.h"

class OrthographicCamera : public ICamera {
 public:
	OrthographicCamera() = default;
	OrthographicCamera(
			float aspect_ratio, float zoom,
			std::pair<float, float> depth_bounds = {0.0f, 1.0f});
	OrthographicCamera(float left, float right, float bottom, float top,
	                   float near = 0.0f, float far = 1.0f);
	void SetPosition(glm::vec3 position) override;
	void SetRotation(glm::quat rotation) override;

	void LookAt(glm::vec3 point_to_view) override;
	void Zoom(float delta) override;
	[[nodiscard]] float zoom_level() const override { return zoom_level_; }
	void SetAspectRatio(float ratio) override;
	[[nodiscard]] float aspect_ratio() const override { return aspect_ratio_; }

	void SetProjection(float left, float right, float bottom, float top,
	                   float near = 0.0f, float far = 1.0f);
	[[nodiscard]] const glm::mat4& viewMatrix() const override {
		return view_matrix_;
	}
	[[nodiscard]] const glm::mat4& projectionMatrix() const override {
		return projection_matrix_;
	}
	[[nodiscard]] const glm::mat4& viewProjectionMatrix() const override {
		return view_projection_matrix_;
	}

 private:
	void RecalculateViewMatrix();
	void RecalculateProjectionMatrix();
	void CalculateViewProjectionMatrix();
	glm::mat4 view_matrix_            = glm::mat4(1.0f);
	glm::mat4 projection_matrix_      = glm::mat4(1.0f);
	glm::mat4 view_projection_matrix_ = glm::mat4(1.0f);

	float zoom_level_   = 1.0f;
	float aspect_ratio_ = 1.0f;
	float near_         = 0.0f;
	float far_          = 1.0f;
};
