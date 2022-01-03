#include "renderer_widget.h"

#include <spdlog/spdlog.h>

#include <QOpenGLContext>

#include "i_scene_viewport.h"

QSurfaceFormat surface_format(QSurfaceFormat::FormatOptions options = {});

RendererWidget::RendererWidget(QWidget* parent)
		: QOpenGLWidget(parent), renderer_(nullptr) {
	setMinimumSize(480, 360);
#ifdef NDEBUG
	setFormat(surface_format());
#else
	setFormat(surface_format(QSurfaceFormat::DebugContext));
#endif
}

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
	// TODO: reuse the pointer
	renderer_ = ISceneViewport::Create(ISceneViewport::API::OpenGL);
	renderer_->Initialize(width(), height());
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
	        [&renderer = *renderer_]() { renderer.ClearResources(); });
}

void RendererWidget::paintGL() {
	renderer_->RenderFrame();
	update();
}

void RendererWidget::resizeGL(const int w, const int h) {
	renderer_->Resize(w, h);
	update();
}

void RendererWidget::RenderShapes(const Core::Shapes& shapes) {
	assert(!shapes.empty() && "no shapes received");
	makeCurrent();
	renderer_->ClearScene();
	renderer_->RenderShapes(shapes);
	doneCurrent();
};

// TODO: maybe need to delegate tweaks to renderer
QSurfaceFormat surface_format(const QSurfaceFormat::FormatOptions options) {
	QSurfaceFormat surface_format = QSurfaceFormat(options);
	surface_format.setVersion(4, 3);
	surface_format.setProfile(QSurfaceFormat::CoreProfile);
	surface_format.setRenderableType(QSurfaceFormat::OpenGL);
	surface_format.setSwapBehavior(QSurfaceFormat::TripleBuffer);
	// One swap per screen update (classic vsync)
	surface_format.setSwapInterval(1);
	// Sizes in bits
	surface_format.setDepthBufferSize(24);
	surface_format.setStencilBufferSize(8);
	return surface_format;
}
