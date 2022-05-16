#pragma once

#include <QMouseEvent>
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
	RendererWidget(QWidget* parent, const std::shared_ptr<renderer::Scene>& scene);
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
	// wheel move = move
	void mousePressEvent(QMouseEvent* event) override;
	void mouseReleaseEvent(QMouseEvent* event) override;
	void mouseMoveEvent(QMouseEvent* event) override;
	void mouseDoubleClickEvent(QMouseEvent* event) override;
#if QT_CONFIG(wheelevent)
	void wheelEvent(QWheelEvent* event) override;
#endif
 public slots:
	void onVisualizationOptionsChanged(
			const VisualizationOptions& visualization_options) {
		UpdateVisualizationOptions(visualization_options);
	};
	void OnChangeVisibility(GlobalIds shape_ids, bool is_visible) {
		viewport_->SetVisibility(shape_ids, is_visible);
	}
	void OnSetSelection(GlobalIds shape_ids, HighlightType type) {
		viewport_->SetSelection(shape_ids, type);
	}
	void OnClearSelection() { 
		viewport_->ClearSelection();
	}
 private:
	Qt::MouseButton current_pressed_button_ = Qt::NoButton;
	QPoint previous_mouse_pos_;
	std::unique_ptr<renderer::ISceneViewport> viewport_;
	std::weak_ptr<renderer::Scene> scene_;
	DrawMode draw_mode_ = DrawMode::Gradient;
};
