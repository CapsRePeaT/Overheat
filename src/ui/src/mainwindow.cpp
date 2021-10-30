#include "mainwindow.h"

#include <QFileDialog>

#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget* parent)
		: QMainWindow(parent), ui_(new Ui::MainWindow) {
	ui_->setupUi(this);
	mdi_area_ = new QMdiArea(this);
	mdi_area_->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	mdi_area_->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
	setCentralWidget(mdi_area_);
	// signals and slots connection
	connect(ui_->load_file_btn, &QAction::triggered, this,
	        &MainWindow::OnLoadFileBtnPressed);
}

MainWindow::~MainWindow() { delete ui_; }

void MainWindow::LoadFile(const std::string& file_name) {
	core().LoadGeometry(file_name);

	mdi_area_->closeAllSubWindows();
	render_widget_ = new RendererWidget(this);
	mdi_area_->addSubWindow(render_widget_);
	render_widget_->show();

	render_widget_->RenderShapes(core().GetShapes());
}

void MainWindow::OnLoadFileBtnPressed() {
	const QString file_name = QFileDialog::getOpenFileName(
			this, tr("Open File"), QDir::currentPath(), tr("geom (*.cpp)"));
	LoadFile(file_name.toStdString());
}
