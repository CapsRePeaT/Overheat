#include "camera_controller.h"

namespace renderer {

void SphericalCameraController::SetRadius(const float radius) {
	radius_ = radius;
	ReplaceCamera();
}

void SphericalCameraController::SetLongitude(const float phi) {
	if (phi < 0 || pi2 < phi)
		phi_ = phi - pi2 * (std::floor(phi / pi2));
	else
		phi_ = phi;
	ReplaceCamera();
}

void SphericalCameraController::SetLatitude(const float theta) {
	theta_ = theta;
	ReplaceCamera();
}

void SphericalCameraController::ReplaceCamera() {
	const float x_radius = std::cos(phi_) * std::cos(theta_) * radius_;
	const float y_radius = std::sin(phi_) * std::cos(theta_) * radius_;
	const float z_radius = std::sin(theta_) * radius_;

	const glm::vec4 radius_vector = {x_radius, y_radius, z_radius, 1.0f};
	camera_->SetPosition(transform() * radius_vector);
	camera_->LookAt(position());
}

}  // namespace renderer
