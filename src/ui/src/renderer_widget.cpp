#include "renderer_widget.h"
#include <qsurfaceformat.h>

#include <QOpenGLContext>

#include "i_scene_viewport.h"
#include "log.h"

QSurfaceFormat surface_format(QSurfaceFormat::FormatOptions options = {});

RendererWidget::RendererWidget(QWidget* parent,
                               const std::shared_ptr<renderer::Scene>& scene)
		: QOpenGLWidget(parent), viewport_(nullptr), scene_(scene) {
	setMinimumSize(480, 360);
#ifdef NDEBUG
	setFormat(surface_format());
#else
	setFormat(surface_format(QSurfaceFormat::DebugContext));
#endif
}

RendererWidget::RendererWidget(QWidget* parent)
		: RendererWidget(parent, nullptr) {}

RendererWidget::~RendererWidget() {
	// Context destroying is guaranted to happen after OpenGLWidget destruction
	// (see https://doc.qt.io/qt-6/qopenglwidget.html#dtor.QOpenGLWidget).
	// Hence, in order to avoid SEGFAULT on signal invoking (because it connected
	// to *this) we must disconnect the signal.
	// Also we can explicitly reset the pointer, or call ClearResources, but it
	// will be done anyway as a part of RendererWidget destruction
	disconnect(context(), &QOpenGLContext::aboutToBeDestroyed, this, nullptr);
	spdlog::debug("Render widget destroyed");
}

void RendererWidget::initializeGL() {
	// We need know which scene to link to viewport after creating RendererWidget,
	// hence we have to store it in class.
	// As RendererWidget is considered to be owned by somthing, that also owns
	// Scene, we can store weak_ptr here.
	assert(!scene_.expired() && "Scene was destroyed before RendererWidget");
	auto&& scene = scene_.lock();
	// TODO: reuse the pointer
	viewport_ =
			renderer::ISceneViewport::Create(renderer::API::OpenGL, std::move(scene));
	viewport_->Initialize(width(), height());
	// Gracefully clear resources of renderer when context is destroying
	// This is needed in cases, when we manually destroying context in a
	// lifetime of one RenderWidget (that is unlikely, but still).
	// For additional information see
	// https://doc.qt.io/qt-6/qopenglcontext.html#aboutToBeDestroyed
	//
	// Also this can be done with renderer_.reset(), because:
	//  1. its resource cleaning must be called in dtors of IRenderer
	//     subclasses too;
	//  2. on next context initializing we reassign unique_ptr anyway
	//     (see 1st line of this function).
	// But 1. I want to reuse the pointer in near future (TBT) and
	// 2. this is more self-documented
	connect(context(), &QOpenGLContext::aboutToBeDestroyed, this,
	        [&renderer = *viewport_]() { renderer.ClearResources(); });
}

void RendererWidget::paintGL() {
	viewport_->RenderFrame();
	update();
}

void RendererWidget::resizeGL(const int w, const int h) {
	viewport_->Resize(w, h);
	update();
}

void RendererWidget::UpdateVisualizationOptions(
		const VisualizationOptions& visualization_options) {
	const auto min_color = visualization_options.min_temp_color;
	const auto max_color = visualization_options.max_temp_color;
	viewport_->SetColorRange(   
			std::array<float, 3>{min_color.redF(), min_color.greenF(),
	                         min_color.blueF()},
			std::array<float, 3>{max_color.redF(), max_color.greenF(),
	                         max_color.blueF()});
	viewport_->SetTemperatureRange(visualization_options.min_temp,
	                               visualization_options.max_temp);
	viewport_->SetDrawMode(visualization_options.draw_mode);
	viewport_->SetStratifiedStep(visualization_options.stratified_step_);

};

// TODO: maybe need to delegate tweaks to renderer
QSurfaceFormat surface_format(const QSurfaceFormat::FormatOptions options) {
	QSurfaceFormat surface_format = QSurfaceFormat(options);
	surface_format.setVersion(4, 3);
	surface_format.setProfile(QSurfaceFormat::CoreProfile);
	surface_format.setRenderableType(QSurfaceFormat::OpenGL);
	surface_format.setSwapBehavior(QSurfaceFormat::TripleBuffer);
	//surface_format.setSamples(16);
	// One swap per screen update (classic vsync)
	surface_format.setSwapInterval(1);
	// Sizes in bits
	surface_format.setDepthBufferSize(24);
	surface_format.setStencilBufferSize(8);
	return surface_format;
}

void RendererWidget::mousePressEvent(QMouseEvent* event) {
	previous_mouse_pos_ = event->pos();
	event->accept();
}

void RendererWidget::mouseReleaseEvent(QMouseEvent* event) { event->accept(); }

void RendererWidget::mouseMoveEvent(QMouseEvent* event) {
	auto clicked_buttons = event->buttons();
	// if button still pressed we use it and don't mention other clicked buttons
	if (current_pressed_button_ & clicked_buttons)
		clicked_buttons = current_pressed_button_;
	bool action_required = true;
	const auto q_curr_pos = event->pos();
	const auto q_delta = q_curr_pos - previous_mouse_pos_;
	const renderer::Vec2D curr_pos{q_curr_pos.x(), q_curr_pos.y()};
	const renderer::Vec2D delta{q_delta.x(), q_delta.y()};
	// rotate
	if ((clicked_buttons & Qt::RightButton) && action_required) {
		current_pressed_button_ = Qt::RightButton;
		action_required        = false;
		viewport_->RotateCamera(curr_pos, delta);
	}
	// move
	if ((clicked_buttons & Qt::MiddleButton) && action_required) {
		current_pressed_button_ = Qt::RightButton;
		action_required        = false;
		viewport_->MoveCamera(curr_pos, delta);
	}
	previous_mouse_pos_ = q_curr_pos;
	event->accept();
}

void RendererWidget::mouseDoubleClickEvent(QMouseEvent* event) {
	event->accept();
}

void RendererWidget::wheelEvent(QWheelEvent* event) {
	// TODO play with
	const float sensitivity = 1.0f;
	const float delta       = event->angleDelta().y() * sensitivity;
	viewport_->ZoomView(delta);
	event->accept();
}

