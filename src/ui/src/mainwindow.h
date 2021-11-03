#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMdiArea>
#include <QDockWidget>

#include "renderer_widget.h"
#include "visualisation_options.h"
#include "metadata_widget.h"
#include "shape_list_widget.h"

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
	VisualisationOptionsWidget* visualisation_options_ = nullptr;
  MetadataWidget* metadata_widget_ = nullptr;
  ShapeListWidget* shape_list_widget_ = nullptr;
	Ui::MainWindow* ui_;
};

#endif  // MAINWINDOW_H
