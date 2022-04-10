#include "mainwindow.h"

#include <QFileDialog>
#include <QMessageBox>

#include "log.h"
#include "scene.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget* parent)
		: QMainWindow(parent),
			scene_(std::make_shared<Scene>()),
			render_widget_(new RendererWidget(this, scene_)),
			visualization_options_(new VisualizationOptionsWidget(this)),
			metadata_widget_(new MetadataWidget(this)),
			shape_list_widget_(new ShapeListWidget(this)),
			ui_(new Ui::MainWindow) {
	ui_->setupUi(this);
	setWindowTitle(tr("OVERHEAT application"));
	// renderer widget
	setCentralWidget(render_widget_);
	render_widget_->showMaximized();
	// shape widget
	addDockWidget(Qt::LeftDockWidgetArea, shape_list_widget_);
	shape_list_widget_->setWindowTitle(tr("Shapes"));
	// metadata widget
	auto* metadata_dock = new QDockWidget(tr("Metadata"), this);
	metadata_dock->setWidget(metadata_widget_);
	addDockWidget(Qt::LeftDockWidgetArea, metadata_dock);
	connect(this, &MainWindow::ShowMetadata, metadata_widget_,
	        &MetadataWidget::OnShowMetadata);
	// options widget
	addDockWidget(Qt::LeftDockWidgetArea, visualization_options_);
	visualization_options_->setWindowTitle(tr("Options"));
	// signals and slots connection
	connect(ui_->load_file_btn, &QAction::triggered, this,
	        &MainWindow::OnLoadFileBtnPressed);
	connect(visualization_options_,
	        &VisualizationOptionsWidget::VisualizationOptionsChanged,
	        render_widget_, &RendererWidget::onVisualizationOptionsChanged);
}

MainWindow::~MainWindow() = default;

void MainWindow::LoadFile(std::string trm_file_path,
                          std::string t2d_file_path) {
	core().LoadGeometry(trm_file_path, std::move(t2d_file_path));
	const auto loaded_shapes   = core().GetFirstFile().GetShapes();
	const auto loaded_heatmaps = core().GetFirstFile().heatmaps();
	// Do we really need this assert?
	// assert(!loaded_shapes.empty() && "no shapes received");
	if (!loaded_shapes.empty()) {
		LOG_INFO("Got {} files to render", loaded_shapes.size());
		// FIXME: Adding shapes shouldn't need to make API context current
		render_widget_->makeCurrent();
		scene_->Clear();
		scene_->AddShapes(loaded_shapes);
		scene_->AddHeatmaps(loaded_heatmaps);
		render_widget_->doneCurrent();
	} else {
		LOG_WARN("No shaped received from file {}", trm_file_path);
	}
}

void MainWindow::OnLoadFileBtnPressed() {
	const QString trm_file = QFileDialog::getOpenFileName(
			this, tr("Open trm File"), QDir::currentPath(),
			tr("geom (*.txt *.TRM);; ALL (*.*)"));
	if (trm_file.isEmpty())
		return;
	QDir trm_dir = QFileInfo(trm_file).absoluteDir();
	QString absolute_trm_dir = trm_dir.absolutePath();
	const QString t2d_file = QFileDialog::getOpenFileName(this, tr("Open T2D File"), 
			absolute_trm_dir, tr("geom (*.txt *.T2D);; ALL (*.*)"));
	// TODO: check if file_name is empty (on cancel)
	if (trm_file.length() && t2d_file.length()) {
		try {
			LoadFile(trm_file.toStdString(), t2d_file.toStdString());
		} catch (...) {
			QMessageBox::critical(
					nullptr, "Error",
					"Unknown error. File cannot be parsed, please check file format.");
		}
	}
}

void MainWindow::OnShapeSelected(const GlobalShapeIds& shape_ids) {
	// TODO: Needed to rework algorithm because it obviously is not optimal
	if (shape_ids == selected_shape_ids_) {
		++selected_shape_index_;
		if (selected_shape_index_ >= shape_ids.size())
			selected_shape_index_ = 0;
	} else {
		selected_shape_ids_   = shape_ids;
		selected_shape_index_ = 0;
	}
	emit ShowMetadata(
			core().GetShapeMetadata(selected_shape_ids_[selected_shape_index_]));
}
