#include "mainwindow.h"

#include <QFileDialog>

#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget* parent)
		: QMainWindow(parent), ui_(new Ui::MainWindow) {
	ui_->setupUi(this);
	setWindowTitle(tr("OVERHEAT application"));
	// renderer widget
	render_widget_ = new RendererWidget(this);
	setCentralWidget(render_widget_);
	render_widget_->showMaximized();
	// shape widget
  shape_list_widget_ = new ShapeListWidget(this);
  addDockWidget(Qt::LeftDockWidgetArea, shape_list_widget_);
  shape_list_widget_->setWindowTitle("Shapes");
	// metadata widget
  metadata_widget_ = new MetadataWidget(this);
  addDockWidget(Qt::LeftDockWidgetArea, metadata_widget_);
  metadata_widget_->setWindowTitle("Metadata");
	// options widget
  visualisation_options_ = new VisualisationOptionsWidget(this);
  addDockWidget(Qt::LeftDockWidgetArea, visualisation_options_);
  visualisation_options_->setWindowTitle("Options");
  // signals and slots connection
	connect(ui_->load_file_btn, &QAction::triggered, this,
	        &MainWindow::OnLoadFileBtnPressed);
}

MainWindow::~MainWindow() { delete ui_; }

void MainWindow::LoadFile(const std::string& file_name) {
	core().LoadGeometry(file_name);
	render_widget_->RenderShapes(core().GetShapes());
}

void MainWindow::OnLoadFileBtnPressed() {
	const QString file_name = QFileDialog::getOpenFileName(
			this, tr("Open File"), QDir::currentPath(), tr("geom (*.cpp)"));
	LoadFile(file_name.toStdString());
}
