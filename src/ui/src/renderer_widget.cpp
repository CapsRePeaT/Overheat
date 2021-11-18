#include "renderer_widget.h"

#include <spdlog/spdlog.h>

#include <QOpenGLContext>

#include "gl_renderer.h"

QSurfaceFormat defaultFormat();
QSurfaceFormat debugFormat();

RendererWidget::RendererWidget(QWidget* parent)
		: QOpenGLWidget(parent), renderer_(nullptr) {
	setMinimumSize(480, 360);
#ifdef NDEBUG
	setFormat(defaultFormat());
#else
	setFormat(debugFormat());
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
	renderer_ = std::make_unique<GLRenderer>();
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
	// But I want to reuse the pointer in near future (TBT) and 
	// this is more self-documented
	connect(context(), &QOpenGLContext::aboutToBeDestroyed, this,
	        [&renderer = *renderer_]() { renderer.ClearResources(); });
}

void RendererWidget::paintGL() {
	renderer_->RenderFrame();
	update();
}

void RendererWidget::resizeGL(int w, int h) {
	renderer_->Resize(w, h);
	update();
}

// TODO: maybe need to delegate tweaks to renderer
QSurfaceFormat defaultFormat() {
	auto surface_format = QSurfaceFormat();
	surface_format.setVersion(4, 3);
	surface_format.setProfile(QSurfaceFormat::CoreProfile);
	surface_format.setRenderableType(QSurfaceFormat::OpenGL);
	surface_format.setSwapBehavior(QSurfaceFormat::TripleBuffer);
	surface_format.setSwapInterval(1);
	surface_format.setDepthBufferSize(24);
	surface_format.setStencilBufferSize(8);

	return surface_format;
}

QSurfaceFormat debugFormat() {
	auto surface_format = QSurfaceFormat(QSurfaceFormat::DebugContext);
	surface_format.setVersion(4, 3);
	surface_format.setProfile(QSurfaceFormat::CoreProfile);
	surface_format.setRenderableType(QSurfaceFormat::OpenGL);
	surface_format.setSwapBehavior(QSurfaceFormat::TripleBuffer);
	surface_format.setSwapInterval(1);
	surface_format.setDepthBufferSize(24);
	surface_format.setStencilBufferSize(8);

	return surface_format;
}
