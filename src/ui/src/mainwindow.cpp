#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui_(new Ui::MainWindow)
{
    ui_->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui_;
}

void MainWindow::LoadFile(const std::string& file_name) {
  core().LoadGeometry(file_name);
  render_widget_->RenderShapes(core().LoadShapes());
}

void MainWindow::OnLoadFile(const std::string& file_name) {
  LoadFile(file_name);
}
