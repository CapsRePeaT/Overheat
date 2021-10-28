#include <QGridLayout>
#include <QLabel>
#include <QFileDialog>
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent), ui_(new Ui::MainWindow) {
  ui_->setupUi(this);
  mdi_area_ = new QMdiArea(this);
  mdi_area_->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
  mdi_area_->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
  setCentralWidget(mdi_area_);
  // signals and slots connection
  connect(ui_->load_file_btn, &QAction::triggered,
          this, &MainWindow::OnLoadFileBtnPressed);
}

MainWindow::~MainWindow() {
  delete ui_;
}

void MainWindow::LoadFile(const std::string& file_name) {
  core().LoadGeometry(file_name);
  render_widget_->RenderShapes(core().GetShapes());
}

void MainWindow::OnLoadFileBtnPressed() {
  const QString file_name = QFileDialog::getOpenFileName(this, tr("Open File"),
                                                  QDir::currentPath(),
                                                  tr("geom (*.cpp)"));
  LoadFile(file_name.toStdString());
}

/*
void MainWindow::OnActionAddWindowTriggered() {
  QWidget* widget = new QWidget(mdi_area_);
  QGridLayout* grid_layout = new QGridLayout(widget);
  widget->setLayout(grid_layout);
  QLabel* label = new QLabel("Hello, I am sub window!!!", widget);
  grid_layout->addWidget(label);
  mdi_area_->addSubWindow(widget);
  widget->setWindowTitle("Sub Window");
  widget->show();
}
*/
