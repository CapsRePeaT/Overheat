#pragma once

#include <QOpenGLWidget>
#include <memory>

#include "common.h"
#include "core.h"
#include "i_scene_viewport.h"
#include "visualization_options.h"

class RendererWidget : public QOpenGLWidget {
	Q_OBJECT
 public:
	explicit RendererWidget(QWidget* parent = nullptr);
	~RendererWidget() override;

	void RenderShapes(const Core::Shapes& shapes);
	void UpdateVisualizationOptions(
			const VisualizationOptions& visualization_options);
	void initializeGL() override;
	void paintGL() override;
	void resizeGL(int w, int h) override;

 public slots:
	void onVisualizationOptionsChanged(
			const VisualizationOptions& visualization_options){};

 private:
	std::unique_ptr<ISceneViewport> renderer_;
	DrawMode draw_mode_ = DrawMode::Gradient;
};
