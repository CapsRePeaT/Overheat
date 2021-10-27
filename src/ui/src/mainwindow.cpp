#include <QGridLayout>
#include <QLabel>
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget* parent) :
    QMainWindow(parent),
    ui_(new Ui::MainWindow){
    ui_->setupUi(this);
    mdi_area_ = new QMdiArea(this);  // init QMdiArea
    // Define scrollbars
    mdi_area_->setHorizontalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    mdi_area_->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    // Set up Mdi Area as a central widget
    setCentralWidget(mdi_area_);
}



MainWindow::~MainWindow(){
    delete ui_;
}

void MainWindow::LoadFile(const std::string& file_name) {
  core().LoadGeometry(file_name);
  render_widget_->RenderShapes(core().LoadShapes());
}

void MainWindow::OnLoadFile(const std::string& file_name) {
  LoadFile(file_name);
}

void MainWindow::OnActionAddWindowTriggered(){
    // Create a widget, that will be a window
    QWidget* widget = new QWidget(mdi_area_);
    // Add a position
    QGridLayout *grid_layout = new QGridLayout(widget);
    widget->setLayout(grid_layout);
    // Add a label
    QLabel *label = new QLabel("Hello, I am sub window!!!", widget);
    grid_layout->addWidget(label);
    // Add widget as a subwindow Mdi Area
    mdi_area_->addSubWindow(widget);
    // Set up a window title
    widget->setWindowTitle("Sub Window");
    // Showing widget
    widget->show();
}
