#include "gl_scene_viewport.h"

#include <glad/glad.h>
#include <spdlog/spdlog.h>

#include <algorithm>
#include <memory>
#include <utility>
#include <vector>

// TODO: maybe rename inner *renderer* folder to *primitives* or smth similar
#include "application/heatmap_material.h"
#include "application/scene_shape.h"
#include "camera_controller.h"
#include "constants.h"
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
	OpenGlInit(w, h);
	ApplicationInit(w, h);
	//#ifndef NDEBUG
	//  DebugInit(w, h);
	//#endif

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
	camera_controller_ = std::make_unique<SphericalCameraController>(
			std::move(camera), 100.0f, glm::pi<float>());
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

void GLSceneViewport::ClearResources() { ClearResourcesImpl(); }

void GLSceneViewport::ClearResourcesImpl() {
	data_.reset();
	if (heatmap_materials_)
		heatmap_materials_->clear();
	scene_->Clear();
	LOG_DEBUG("Context cleared");
}

void GLSceneViewport::RenderFrame() {
	auto& api = RendererAPI::instance();
	api.Clear();

	const ICamera& camera = camera_controller_->camera();

	if (!heatmap_materials_ && !scene_->heatmaps().empty()) {
		auto heatmaps      = scene_->heatmaps();
		heatmap_materials_ = std::vector<HeatmapMaterial>();
		heatmap_materials_->reserve(heatmaps.size() - 1);
		auto& factory = RendererAPI::factory();
		for (size_t i = 0; i < heatmaps.size() - 1; ++i) {
			int side_resolution     = heatmaps[i].x_resolution();
			const auto& bot_heatmap = heatmaps[i];
			const auto& top_heatmap = heatmaps[i + 1];

			// static bool is_printed = true;
			// if (!is_printed && i == 6) {
			// 	is_printed = true;
			// 	bot_heatmap.DebugPrint(39);
			// }
			
			auto heatmap_bottom_texture =
					factory.NewTexture2D(side_resolution, side_resolution,
			                         bot_heatmap.temperatures().data(), 1);
			auto heatmap_top_texture =
					factory.NewTexture2D(side_resolution, side_resolution,
			                         top_heatmap.temperatures().data(), 1);

			auto texture_pair =
					std::pair<std::unique_ptr<Texture2D>, std::unique_ptr<Texture2D>>(
							std::move(heatmap_bottom_texture),
							std::move(heatmap_top_texture));
			auto temp_ranges_pair = std::pair<glm::vec2, glm::vec2>(
					{bot_heatmap.min_temp(), bot_heatmap.max_temp()},
					{top_heatmap.min_temp(), top_heatmap.max_temp()});

			heatmap_materials_->emplace_back(std::move(texture_pair),
			                                 temp_ranges_pair);
		}
	}

	if (heatmap_materials_)
		for (const auto& shape : scene_->shapes()) {
			// LOG_TRACE("Render shape: id {}, layer {}", shape->id().id(),
			// shape->layer_id());
			const auto& bbox = shape->core_shape().bbox().coordinates();
			const glm::mat2 bounds = {{bbox[0].first, bbox[1].first}, {bbox[0].second, bbox[1].second}};
			(*heatmap_materials_)[shape->core_shape().layer_id()].Use(
					shape->transform(), camera.viewProjectionMatrix(), bounds);
			api.DrawIndexed(shape->vertex_array());
		}

	if (data_) {
		const auto& axes = data_->axes;
		data_->debug_material->Use(axes->transform(), camera.viewProjectionMatrix(),
		                           1.0f);
		api.DrawIndexed(axes->vertex_array(), PrimitiveType::LINES);
	}
}

void GLSceneViewport::Resize(const int w, const int h) {
	RendererAPI::instance().SetViewPort(0, 0, w, h);
	camera_controller_->SetCameraAspectRatio(static_cast<float>(w) /
	                                         static_cast<float>(h));
}

void GLSceneViewport::SetTemperatureRange(const float min, const float max) {
	if (heatmap_materials_)
		std::ranges::for_each(*heatmap_materials_, [&min, &max](auto& material) {
			material.SetTemperatureRange(min, max);
		});
}

void GLSceneViewport::SetColorRange(const ISceneViewport::Color min,
                                    const ISceneViewport::Color max) {
	if (heatmap_materials_)
		std::ranges::for_each(*heatmap_materials_, [&min, &max](auto& material) {
			material.SetColorRange({min[0], min[1], min[2]},
			                       {max[0], max[1], max[2]});
		});
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
	static constexpr float zoom_coefficient = 1.0f / 1500.0f;
	const float zoom_level = camera_controller_->camera().zoom_level();

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

}  // namespace renderer
