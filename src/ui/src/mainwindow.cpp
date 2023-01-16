#include "mainwindow.h"

#include <QFileDialog>
#include <QMessageBox>

#include "log.h"
#include "scene.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget* parent)
		: QMainWindow(parent),
			scene_(std::make_shared<renderer::Scene>()),
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
	connect(shape_list_widget_, &ShapeListWidget::ShowMetadata, 
		      this, &MainWindow::OnShowMetadata);
	connect(shape_list_widget_, &ShapeListWidget::ShapesSelected, 
		      this, &MainWindow::OnShapesSelected);
//	connect(shape_list_widget_, &ShapeListWidget::ChangeVisibility, 
//		      this, &MainWindow::OnShowMetadata);
	// metadata widget
	auto* metadata_dock = new QDockWidget(tr("Metadata"), this);
	metadata_dock->setWidget(metadata_widget_);
	addDockWidget(Qt::LeftDockWidgetArea, metadata_dock);
	// options widget
	addDockWidget(Qt::LeftDockWidgetArea, visualization_options_);
	visualization_options_->setWindowTitle(tr("Options"));
	// signals and slots connection
	connect(ui_->load_file_3d_btn, &QAction::triggered, this,
	        &MainWindow::OnLoadFile3DBtnPressed);
	connect(ui_->load_file_2d_btn, &QAction::triggered, this,
	        &MainWindow::OnLoadFile2DBtnPressed);
	connect(visualization_options_, &VisualizationOptionsWidget::VisualizationOptionsChanged,
	        render_widget_, &RendererWidget::onVisualizationOptionsChanged);
	connect(shape_list_widget_, &ShapeListWidget::ChangeVisibility,
	        render_widget_, &RendererWidget::OnChangeVisibility);
	core(); // for debug
}

MainWindow::~MainWindow() = default;

void MainWindow::LoadFile(std::string trm_file_path,
                          std::string t2d_file_path,
                          const GeometryType type) {
	const auto rep_id = core().LoadRepresentation(trm_file_path, std::move(t2d_file_path), type);
	auto& representation  = core().GetRepresentation(rep_id);
	const auto loaded_shapes   = representation.GetShapes();
	const auto loaded_heatmaps = representation.heatmaps();
	shape_list_widget_->AddData(core().GetRepresentationData(rep_id.representation_id()));
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
		visualization_options_->SetMinMaxTemp(loaded_heatmaps.min_temp(),
																					loaded_heatmaps.max_temp());
		
	} else {
		LOG_WARN("No shaped received from file {}", trm_file_path);
	}
}

void MainWindow::OnLoadFile3DBtnPressed() {
	GetFilesAndLoad(GeometryType::D3);
}

void MainWindow::OnLoadFile2DBtnPressed() { 
	GetFilesAndLoad(GeometryType::D2); 
}

void MainWindow::GetFilesAndLoad(const GeometryType type) {
	const QString trm_file = QFileDialog::getOpenFileName(
			this, tr("Open trm File"), QDir::currentPath(),
			tr("geom (*.txt *.TRM);; ALL (*.*)"));
	if (trm_file.isEmpty())
		return;
	QDir trm_dir = QFileInfo(trm_file).absoluteDir();
	QString absolute_trm_dir = trm_dir.absolutePath();
	const QString t2d_file = QFileDialog::getOpenFileName(this, tr("Open T2D File"),
	                                                      absolute_trm_dir,
		                                                    tr("geom (*.txt *.T2D);; ALL (*.*)"));
	if (trm_file.length() && t2d_file.length()) {
//		try {
			LoadFile(trm_file.toStdString(), t2d_file.toStdString(), type);
//		} catch (...) {
//			QMessageBox::critical(
//					nullptr, "Error",
//					"Unknown error. File cannot be parsed, please check file format.");
//		}
	}
}

void MainWindow::OnShapesSelected(const GlobalShapeIds& shape_ids) {
	// TODO: Needed to rework algorithm because it obviously is not optimal
	if (!shape_ids.size()) {
		render_widget_->OnClearSelection();
		return;
	}
		
	if (std::ranges::equal(shape_ids, selected_shape_ids_)) {
		++selected_shape_index_;
		if (selected_shape_index_ >= shape_ids.size())
			selected_shape_index_ = 0;
	} else {
		selected_shape_ids_   = shape_ids;
		selected_shape_index_ = 0;
	}
	const auto selected_shape_id = selected_shape_ids_[selected_shape_index_];
	render_widget_->OnClearSelection();
	render_widget_->OnSetSelection(shape_ids, HighlightType::Selected);
	render_widget_->OnSetSelection({selected_shape_id}, HighlightType::ActiveSelected);
	OnShowMetadata(selected_shape_id);
}

void MainWindow::OnShowMetadata(GlobalId id) {
	metadata_widget_->ShowMetadata(core().GetMetadata(id));
}
