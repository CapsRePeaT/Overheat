#include "shape_list_widget.h"

#include "ui_shape_list_widget.h"

ShapeListWidget::ShapeListWidget(QWidget* parent)
		: QDockWidget(parent), ui(new Ui::ShapeListWidget) {
	ui->setupUi(this);
}

ShapeListWidget::~ShapeListWidget() { delete ui; }
