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
 public slots:
	void OnLoadFileBtnPressed();

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
};
