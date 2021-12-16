#pragma once

#include <QDockWidget>
#include <QMainWindow>
#include <QMdiArea>

#include "metadata_widget.h"
#include "renderer_widget.h"
#include "shape_list_widget.h"
#include "visualization_options.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow {
	Q_OBJECT
 public:
	explicit MainWindow(QWidget* parent = nullptr);
	void LoadFile(const std::string& file_name);
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
	VisualizationOptionsWidget* visualization_options_ = nullptr;
	MetadataWidget* metadata_widget_ = nullptr;
	ShapeListWidget* shape_list_widget_ = nullptr;
	Ui::MainWindow* ui_;
};
