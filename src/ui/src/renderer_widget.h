#pragma once

#include <QOpenGLWidget>
#include <QMouseEvent>
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
	// camera controls 
	// Right btn pressed + move = rotate
	// left btn = selection
	// Middle btn pressed + move = pan
	// weel move = move
	virtual void mousePressEvent(QMouseEvent* event) override;
	virtual void mouseReleaseEvent(QMouseEvent* event) override;
	virtual void mouseMoveEvent(QMouseEvent* event) override;
	virtual void mouseDoubleClickEvent(QMouseEvent* event) override;
#if QT_CONFIG(wheelevent)
	virtual void wheelEvent(QWheelEvent* event) override;
#endif
 public slots:
	void onVisualizationOptionsChanged(
			const VisualizationOptions& visualization_options){
		UpdateVisualizationOptions(visualization_options);
	};

 private:
	Qt::MouseButton current_pressed_button_ = Qt::NoButton;
	QPoint previous_mouse_pos_; 
	std::unique_ptr<ISceneViewport> viewport_;
	std::weak_ptr<Scene> scene_;
	DrawMode draw_mode_ = DrawMode::Gradient;
};
