#include "shape_list_widget.h"

#include "ui_shape_list_widget.h"

ShapeListWidget::ShapeListWidget(QWidget* parent)
		: QDockWidget(parent), ui_(new Ui::ShapeListWidget) {
	ui_->setupUi(this);
}

ShapeListWidget::~ShapeListWidget() { delete ui_; }
