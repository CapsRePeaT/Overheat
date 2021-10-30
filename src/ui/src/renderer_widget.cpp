#include "renderer_widget.h"

#include <spdlog/spdlog.h>

#include <QOpenGLContext>

#include "gl_renderer.h"

QSurfaceFormat defaultFormat();
QSurfaceFormat debugFormat();

RendererWidget::RendererWidget(QWidget* parent)
		: QOpenGLWidget(parent), renderer_(std::make_unique<GLRenderer>()) {
	setMinimumSize(480, 360);
#ifdef NDEBUG
	setFormat(defaultFormat());
#else
	setFormat(debugFormat());
#endif
}

RendererWidget::~RendererWidget() {
	renderer_->Clear();
	spdlog::debug("Render widget destroyed");
}

void RendererWidget::initializeGL() {
	renderer_->Initialize(width(), height());
}

void RendererWidget::paintGL() { renderer_->RenderFrame(); }

void RendererWidget::resizeGL(int w, int h) { renderer_->Resize(w, h); }

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
