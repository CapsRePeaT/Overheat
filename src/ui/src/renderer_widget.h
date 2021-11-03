#ifndef RENDERER_WIDGET_H
#define RENDERER_WIDGET_H

#include <QOpenGLWidget>
#include <memory>

#include "common.h"
#include "core.h"
#include "i_renderer.h"
#include "visualisation_options.h"

class RendererWidget : public QOpenGLWidget {
	Q_OBJECT
 public:
	explicit RendererWidget(QWidget* parent = nullptr);
	~RendererWidget() override;

	void RenderShapes(const Core::Shapes& shapes) {
		assert(!shapes.empty() && "no shapes received");
		renderer_->RenderShapes(shapes);
	};
	void UpdateVisualisationOptions(
			const VisualisationOptions& visualisation_options);

	void initializeGL() override;
	void paintGL() override;
	void resizeGL(int w, int h) override;

 public slots:
	void onVisualisationOptionsChanged(
			const VisualisationOptions& visualisation_options){};

 private:
	std::unique_ptr<IRenderer> renderer_;
	DrawMode draw_mode_ = DrawMode::Gradient;
};

#endif  // RENDERER_WIDGET_H
