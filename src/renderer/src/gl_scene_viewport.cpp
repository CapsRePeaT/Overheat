#include "gl_scene_viewport.h"

#include <glad/glad.h>
#include <spdlog/spdlog.h>

#include <algorithm>
#include <cassert>
#include <memory>
#include <utility>
#include <vector>

// TODO: maybe rename inner *renderer* folder to *primitives* or smth similar
#include "application/heatmap_material.h"
#include "application/scene_shape.h"
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

void GLSceneViewport::InitTemperatureBar(const float min_temp, const float max_temp) {
	glm::vec2 temperature_bar_size = {tbar_thickness_,
	                                  static_cast<float>(view_size_.y) - 2 * tbar_screen_margins_.y};

	temperature_bar_ = std::make_unique<TemperatureBar>(
			temperature_bar_size, tbar_screen_margins_, font_, min_temp, max_temp);
}

void GLSceneViewport::ClearResources() { ClearResourcesImpl(); }

glm::vec3 HighlightTypeToColor(HighlightType highlight_type) {
	switch (highlight_type) {
		case HighlightType::None:
			return consts::color_not_selected;
		case HighlightType::Selected:
			return consts::color_selected;
		case HighlightType::ActiveSelected:
			return consts::color_active_selected;
		default:
			assert(false && "Unkown highlight type!");
			return consts::vec3_0;
	}
}

void GLSceneViewport::ClearResourcesImpl() {
	data_.reset();
	if (heatmap_materials_)
		heatmap_materials_->clear();
	scene_->Clear();
	LOG_DEBUG("Context cleared");
}

void GLSceneViewport::RenderFrame() {
	static int count = 0;
	if (count++ < 2) return;
	auto& api = RendererAPI::instance();
	api.Clear();

	const ICamera& camera = camera_controller_->camera();

	if (!heatmap_materials_ && !scene_->heatmaps().empty()) {
		InitHeatmapMaterials();
	}

	if (heatmap_materials_) {
		for (const auto& shape : scene_->shapes()) {
			const auto layer_id        = shape->core_shape().layer_id();
			const auto shape_transform = shape->transform();
			// LOG_TRACE("Render shape: id {}, layer {}", shape->id().id(),
			// shape->layer_id());
			if (shape->is_visible() ||
			    shape->highlight_type() != HighlightType::None) {
				auto& materials = *heatmap_materials_;
				auto& material  = materials[layer_id];
				material.Use(shape_transform, camera.viewProjectionMatrix(),
				             HighlightTypeToColor(shape->highlight_type()),
				             !shape->is_visible());
				api.DrawIndexed(shape->vertex_array());
				material.Unuse();
			}
		}
	}

	if (!temperature_bar_material_)
		temperature_bar_material_ = std::make_unique<TemperatureBarMaterial>();
	temperature_bar_material_->Use(camera.uiViewMatrix());
	// data_->debug_heatmap_material->Use(glm::mat4(1), camera.uiViewMatrix());
	if (!temperature_bar_)
		InitTemperatureBar();
	api.DrawIndexed(temperature_bar_->vertex_array());
	// data_->debug_heatmap_material->Unuse();
	temperature_bar_material_->Unuse();

	auto& labels = temperature_bar_->labels();
	if (!labels.empty()) {
		font_->material().Use(camera.uiViewMatrix());
		for (const auto& text : labels) {
			api.DrawIndexed(text->vertex_array());
		}
		font_->material().Unuse();
	}

	if (data_) {
		const auto& axes = data_->axes;
		data_->debug_material->Use(axes->transform(), camera.viewProjectionMatrix(),
		                           1.0f);
		api.DrawIndexed(axes->vertex_array(), PrimitiveType::LINES);
		data_->debug_material->Unuse();
	}
}

HeatmapMaterial CreateMaterial(const Heatmap& bot_heatmap,
                               const Heatmap& top_heatmap,
                               std::pair<float, float> bounds) {
	auto& factory               = RendererAPI::factory();
	int side_resolution         = bot_heatmap.x_resolution();
	auto heatmap_bottom_texture = factory.NewTexture2D(
			side_resolution, side_resolution, bot_heatmap.temperatures().data(), 1);
	auto heatmap_top_texture = factory.NewTexture2D(
			side_resolution, side_resolution, top_heatmap.temperatures().data(), 1);

	auto texture_pair =
			std::pair<std::unique_ptr<Texture2D>, std::unique_ptr<Texture2D>>(
					std::move(heatmap_bottom_texture), std::move(heatmap_top_texture));
	auto temp_ranges_pair = std::pair<glm::vec2, glm::vec2>(
			{bot_heatmap.min_temp(), bot_heatmap.max_temp()},
			{top_heatmap.min_temp(), top_heatmap.max_temp()});

	return {std::move(texture_pair), temp_ranges_pair, bounds};
}

void GLSceneViewport::InitHeatmapMaterials() {
	auto heatmaps = scene_->heatmaps();
	assert(heatmaps.size());
	heatmap_materials_ = std::vector<HeatmapMaterial>();
	heatmap_materials_->reserve(heatmaps.size() - 1);
	if (heatmaps.size() == 1) {
		const auto& heatmap = heatmaps.front();
		heatmap_materials_->emplace_back(
				CreateMaterial(heatmap, heatmap, scene_->bounds()));
	}
	for (size_t i = 0; i < heatmaps.size() - 1; ++i) {
		const auto& bot_heatmap = heatmaps[i];
		const auto& top_heatmap = heatmaps[i + 1];
		heatmap_materials_->emplace_back(
				CreateMaterial(bot_heatmap, top_heatmap, scene_->bounds()));
	}

	// TODO: move this to more appropriate place
	glm::vec3 center_of_bounds = {scene_->bounds().first, scene_->bounds().second,
	                              0.0f};
	center_of_bounds /= 2;
	camera_controller_->SetPosition(center_of_bounds);
}

void GLSceneViewport::Resize(const int w, const int h) {
	RendererAPI::instance().SetViewPort(0, 0, w, h);
	view_size_ = {w, h};
	camera_controller_->SetCameraAspectRatio(static_cast<float>(w) /
	                                         static_cast<float>(h));
	camera_controller_->SetCameraScreenBounds(w, h);
	// TODO: make transformt matrix for this objects
	InitTemperatureBar();
}

void GLSceneViewport::SetTemperatureRange(const float min, const float max) {
	if (heatmap_materials_)
		std::ranges::for_each(*heatmap_materials_, [&min, &max](auto& material) {
			material.SetTemperatureRange(min, max);
		});
	InitTemperatureBar(min, max);
	// temperature_bar_->SetTemperatureRange(min, max);
}

void GLSceneViewport::SetColorRange(const ISceneViewport::Color min,
                                    const ISceneViewport::Color max) {
	if (heatmap_materials_)
		for (auto& material : *heatmap_materials_)
			material.SetColorRange({min[0], min[1], min[2]},
			                       {max[0], max[1], max[2]});

	temperature_bar_material_->SetColorRange({min[0], min[1], min[2]},
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
	bool is_stratified = false;
	switch (mode) {
		case DrawMode::Gradient:
			is_stratified = false;
			break;
		case DrawMode::Stratified:
			is_stratified = true;
			break;
	}
	if (heatmap_materials_)
		std::ranges::for_each(*heatmap_materials_, [is_stratified](auto& material) {
			material.SetIsStratified(is_stratified);
		});
}
void GLSceneViewport::SetStratifiedStep(float step) {
	if (heatmap_materials_)
		std::ranges::for_each(*heatmap_materials_, [step](auto& material) {
			material.SetStratifiedStep(step);
		});
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
