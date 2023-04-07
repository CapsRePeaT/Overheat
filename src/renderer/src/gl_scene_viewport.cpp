#include "gl_scene_viewport.h"

#include <glad/glad.h>
#include <spdlog/spdlog.h>

#include <algorithm>
#include <cassert>
#include <memory>
#include <utility>
#include <vector>

// TODO: maybe rename inner *renderer* folder to *primitives* or smth similar
#include "application/box_shape.h"
#include "application/heatmap_material.h"
#include "application/temperature_bar.h"
#include "application/temperature_bar_material.h"
#include "camera_controller.h"
#include "common.h"
#include "constants.h"
#include "heatmap.h"
#include "log.h"
#include "misc/formatters.h"
#include "renderer/debug/axes.h"
#include "renderer/debug/debug_heatmap_material.h"
#include "renderer/debug/debug_material.h"
#include "renderer/i_camera.h"
#include "renderer/orthographic_camera.h"
#include "renderer/renderer_api.h"
#include "renderer/texture2d.h"
#include "scene.h"
#include "text/font.h"


namespace renderer {

struct GLSceneViewport::Impl {
	std::unique_ptr<debug::DebugMaterial> debug_material =
			std::make_unique<debug::DebugMaterial>();
	std::unique_ptr<debug::DebugHeatmapMaterial> debug_heatmap_material =
			std::make_unique<debug::DebugHeatmapMaterial>();
	std::unique_ptr<debug::Axes> axes = std::make_unique<debug::Axes>();
};

GLSceneViewport::GLSceneViewport(std::shared_ptr<Scene> scene)
		: scene_(std::move(scene)), data_(nullptr) {}

GLSceneViewport::~GLSceneViewport() { ClearResourcesImpl(); }

void GLSceneViewport::Initialize(const int w, const int h) {
	view_size_ = {w, h};
	OpenGlInit(w, h);
	ApplicationInit(w, h);
	// #ifndef NDEBUG
	//  DebugInit(w, h);
	// #endif
	is_initialized_ = true;
}

void GLSceneViewport::OpenGlInit(const int w, const int h) {
	if (!gladLoadGL())
		LOG_ERROR("Failed to initialize opengl functions");
	LOG_DEBUG("GLAD initialized");

	auto& api = RendererAPI::Init(API::OpenGL);
	api.SetClearColor(consts::init::clear_color);
	api.SetViewPort(0, 0, w, h);
}

void GLSceneViewport::ApplicationInit(const int w, const int h) {
	const float aspect_ratio = static_cast<float>(w) / static_cast<float>(h);

	LOG_DEBUG("Initializing camera: w = {}, h = {}, aspect_ratio = {}, zoom = {}",
	          w, h, aspect_ratio, consts::init::zoom);

	auto camera = std::make_unique<OrthographicCamera>(
			aspect_ratio, consts::init::zoom, consts::init::near_far_bounds);
	camera->SetScreenBounds(w, h);
	camera_controller_ = std::make_unique<SphericalCameraController>(
			std::move(camera), /*radius=*/100.0f, /*phi=*/glm::pi<float>() * 3 / 2,
			/*theta*/ glm::pi<float>() * 0.5f);
	font_ = std::make_unique<Font>(Font::default_font_name, 16);
	if (!font_->Init())
		LOG_CRITICAL("Font has not been initialized");
	InitTemperatureBar();
}

void GLSceneViewport::DebugInit(const int /*w*/, const int /*h*/) {
	data_ = std::make_unique<Impl>();
	data_->axes->ApplyScale(4);
	data_->debug_material     = std::make_unique<debug::DebugMaterial>();
	const Core::Shapes shapes = {std::make_shared<BasicShape>(
			GlobalId(InstanceType::Shape, 0, 0), 0,
			Box3D({{0.0f, 1.0f}, {0.0f, 2.0f}, {0.0f, 3.0f}}))};
	scene_->AddShapes(shapes);
	constexpr glm::vec3 offset = {0.1, 0.1, 0.1};
	scene_->shapes()[0]->Translate(offset);
}

void GLSceneViewport::InitTemperatureBar(const float min_temp,
                                         const float max_temp) {
	glm::vec2 temperature_bar_size = {
			tbar_thickness_,
			static_cast<float>(view_size_.y) - 2 * tbar_screen_margins_.y};

	temperature_bar_ = std::make_unique<TemperatureBar>(
			temperature_bar_size, tbar_screen_margins_, font_, min_temp, max_temp);
}

void GLSceneViewport::ClearResources() { ClearResourcesImpl(); }

void GLSceneViewport::ClearResourcesImpl() {
	data_.reset();
	scene_->Clear();
	LOG_DEBUG("Context cleared");
}

void GLSceneViewport::Draw(RendererContext& ctx, Drawable& drawable) {
	if (drawable.SetContextForDraw(ctx)) {
		auto& api = RendererAPI::instance();
		ctx.UseMaterial();
		api.DrawIndexed(ctx.vao());
		ctx.UnuseMaterial();
	}
}

void GLSceneViewport::RenderFrame() {
	static int count = 0;
	if (count++ < 2)
		return;
	auto& api = RendererAPI::instance();
	api.Clear();
	api.EnableDepthBuffer(true);
	const ICamera& camera = camera_controller_->camera();
	RendererContext ctx(camera);

	if (scene_->UpdateForRenderer()) {
		glm::vec3 center_of_bounds = {scene_->bounds().first,
		                              scene_->bounds().second, 0.0f};
		center_of_bounds /= 2;
		camera_controller_->SetPosition(center_of_bounds);
	}

	for (const auto& shape : scene_->shapes()) {
		Draw(ctx, *shape);
	}

	api.EnableDepthBuffer(false);
	// data_->debug_heatmap_material->Use(glm::mat4(1), camera.uiViewMatrix());
	if (!temperature_bar_)
		InitTemperatureBar();
	Draw(ctx, *temperature_bar_);

	auto& labels = temperature_bar_->labels();
	if (!labels.empty()) {
		for (auto& text : labels) {
			Draw(ctx, *text);
		}
	}

	// care about this not enough to include it to Drawable family
	if (data_) {
		const auto& axes = data_->axes;
		data_->debug_material->Use(axes->transform(), camera.viewProjectionMatrix(),
		                           1.0f);
		api.DrawIndexed(axes->vertex_array(), PrimitiveType::LINES);
		data_->debug_material->Unuse();
	}
}

void GLSceneViewport::Resize(const int w, const int h) {
	RendererAPI::instance().SetViewPort(0, 0, w, h);
	view_size_ = {w, h};
	camera_controller_->SetCameraAspectRatio(static_cast<float>(w) /
	                                         static_cast<float>(h));
	camera_controller_->SetCameraScreenBounds(w, h);
	// TODO: make transformt matrix for this objects
	auto [min_temp, max_temp] = temperature_bar_->temperature_range();
	InitTemperatureBar(min_temp, max_temp);
}

void GLSceneViewport::SetTemperatureRange(const float min, const float max) {
	scene_->SetTemperatureRange(min, max);
	InitTemperatureBar(min, max);
	// temperature_bar_->SetTemperatureRange(min, max);
}

void GLSceneViewport::SetColorRange(const ISceneViewport::Color min,
                                    const ISceneViewport::Color max) {
	scene_->SetColorRange(min, max);
	temperature_bar_->SetColorRange({min[0], min[1], min[2]},
	                                {max[0], max[1], max[2]});
}

void GLSceneViewport::MoveCamera(const Vec2D /*screenPoint*/,
                                 const Vec2D delta) {
	static constexpr float zoom_coefficient = 0.002f;
	const float zoom_level = camera_controller_->camera().zoom_level();

	const float x_coefficient = -zoom_level * zoom_coefficient;
	const float y_coefficient = zoom_level * zoom_coefficient;

	const auto dx = static_cast<float>(delta.x);
	const auto dy = static_cast<float>(delta.y);

	const auto horizontal_translation =
			x_coefficient * dx * camera_controller_->camera().rightVector();
	const auto vertical_translation =
			y_coefficient * dy * camera_controller_->camera().upVector();

	camera_controller_->Translate(horizontal_translation + vertical_translation);
}

void GLSceneViewport::RotateCamera(const Vec2D /*screenPoint*/,
                                   const Vec2D delta) {
	static constexpr float zoom_coefficient = 1.0f / 500.0f;
	const float zoom_level = 1.0f;  // camera_controller_->camera().zoom_level();

	const float y_coefficient = zoom_level * zoom_coefficient;
	const float x_coefficient = -zoom_level * zoom_coefficient;

	const auto dx = static_cast<float>(delta.x);
	const auto dy = static_cast<float>(delta.y);

	camera_controller_->AddLongitude(x_coefficient * dx);
	camera_controller_->AddLatitude(y_coefficient * dy);
}

void GLSceneViewport::ZoomView(const float delta) {
	static constexpr float zoom_base              = 2.0f;
	static constexpr float zoom_delta_coefficient = -1.0f / 180.0f;
	const float zoom = std::pow(zoom_base, delta * zoom_delta_coefficient);

	camera_controller_->Zoom(zoom);
}

void GLSceneViewport::SetVisibility(const GlobalIds& to_change,
                                    bool is_visible) {
	for (auto id : to_change) {
		scene_->shape_by_id(id)->SetIsVisible(is_visible);
	}
}

void GLSceneViewport::SetDrawMode(DrawMode mode) {
	scene_->SetDrawMode(mode);
}

void GLSceneViewport::SetStratifiedStep(float step) {
	scene_->SetStratifiedStep(step);
}

void GLSceneViewport::ClearSelection() {
	std::ranges::for_each(scene_->shapes(), [](auto& shape) {
		shape->SetHighlightType(HighlightType::None);
	});
}

void GLSceneViewport::SetSelection(const GlobalIds& to_change,
                                   HighlightType type) {
	for (auto id : to_change) {
		scene_->shape_by_id(id)->SetHighlightType(type);
	}
}

}  // namespace renderer
