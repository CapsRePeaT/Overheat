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
	RendererWidget(QWidget* parent, std::shared_ptr<Scene> scene);
	~RendererWidget() override;

	void UpdateVisualizationOptions(
			const VisualizationOptions& visualization_options);
	void initializeGL() override;
	void paintGL() override;
	void resizeGL(int w, int h) override;

 public slots:
	void onVisualizationOptionsChanged(
			const VisualizationOptions& visualization_options){};

 private:
	std::unique_ptr<ISceneViewport> viewport_;
	std::weak_ptr<Scene> scene_;
	DrawMode draw_mode_ = DrawMode::Gradient;
};
