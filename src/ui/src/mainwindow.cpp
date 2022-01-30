#include "mainwindow.h"

#include <QFileDialog>
#include <QMessageBox>

#include "log.h"
#include "scene.h"
#include "ui_mainwindow.h"



MainWindow::MainWindow(QWidget* parent)
		: QMainWindow(parent),
			scene_(std::make_shared<Scene>()), 
			ui_(new Ui::MainWindow) {
	ui_->setupUi(this);
	setWindowTitle(tr("OVERHEAT application"));
	// renderer widget
	render_widget_ = new RendererWidget(this, scene_);
	setCentralWidget(render_widget_);
	render_widget_->showMaximized();
	// shape widget
	shape_list_widget_ = new ShapeListWidget(this);
	addDockWidget(Qt::LeftDockWidgetArea, shape_list_widget_);
	shape_list_widget_->setWindowTitle("Shapes");
	// metadata widget
	metadata_widget_ = new MetadataWidget(this);
	QDockWidget* metadata_dock = new QDockWidget(tr("Metadata"), this);
	metadata_dock->setWidget(metadata_widget_);
	addDockWidget(Qt::LeftDockWidgetArea, metadata_dock);

	// options widget
	visualization_options_ = new VisualizationOptionsWidget(this);
	addDockWidget(Qt::LeftDockWidgetArea, visualization_options_);
	visualization_options_->setWindowTitle("Options");
	// signals and slots connection
	connect(ui_->load_file_btn, &QAction::triggered, this,
	        &MainWindow::OnLoadFileBtnPressed);
	connect(visualization_options_,
	        &VisualizationOptionsWidget::VisualizationOptionsChanged,
	        render_widget_, &RendererWidget::onVisualizationOptionsChanged);
}

MainWindow::~MainWindow() { delete ui_; }

void MainWindow::LoadFile(const std::string& file_name) {
	core().LoadGeometry(file_name);
	const auto loaded_shapes = core().GetFirstFile().GetShapes();
	// Do we really need this assert?
	// assert(!loaded_shapes.empty() && "no shapes received");
	if (!loaded_shapes.empty()) {
		LOG_INFO("Got {} files to render", loaded_shapes.size());
		// FIXME: Adding shapes shouldn't need to make API context current
		render_widget_->makeCurrent();
		scene_->Clear();
		scene_->AddShapes(loaded_shapes);
		render_widget_->doneCurrent();
	}
	else {
		LOG_WARN("No shaped received from file {}", file_name);
	}
}

void MainWindow::OnLoadFileBtnPressed() {
	const QString file_name = QFileDialog::getOpenFileName(
			this, tr("Open File"), QDir::currentPath(), tr("geom (*.txt *.TRM);; ALL (*.*)"));
	// TODO: check if file_name is empty (on cancel)
	if (file_name.length()) {
		try {
			LoadFile(file_name.toStdString());
		} catch(...) {
			QMessageBox messageBox;
			messageBox.critical(0, "Error", "Unknown error. File cannot be parsed, please check file format.");
			messageBox.setFixedSize(500, 200);
		}
	}
	
}
