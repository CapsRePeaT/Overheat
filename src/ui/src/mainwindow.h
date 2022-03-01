#pragma once

#include <QDockWidget>
#include <QMainWindow>
#include <QMdiArea>

#include "metadata_widget.h"
#include "renderer_widget.h"
#include "shape_list_widget.h"
#include "visualization_options.h"
#include "scene.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
	Q_OBJECT
 public:
	explicit MainWindow(QWidget* parent = nullptr);
	void LoadFile(const std::string& trm_file, 
								const std::string& t2d_file);
	~MainWindow();
 signals:
	// todo
	void ShowMetadata(const MetadataPack& pack);
 public slots:
	void OnLoadFileBtnPressed();
	void OnShapeSelected(const ShapeIds& shape_ids);

	// private slots:
	//  void OnActionAddWindowTriggered();

 private:
	Core& core() { return Core::instance(); }
	RendererWidget* render_widget_ = nullptr;
	std::shared_ptr<Scene> scene_;
	VisualizationOptionsWidget* visualization_options_ = nullptr;
	MetadataWidget* metadata_widget_ = nullptr;
	ShapeListWidget* shape_list_widget_ = nullptr;
	Ui::MainWindow* ui_;
	// we need to store them for iteration over 
	// selected shapes and providing metadata
	ShapeIds selected_shape_ids_;
	size_t selected_shape_index_ = 0;
};
