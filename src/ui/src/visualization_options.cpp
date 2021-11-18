#include "visualization_options.h"

#include "ui_visualization_options.h"

VisualizationOptionsWidget::VisualizationOptionsWidget(QWidget* parent)
    : QDockWidget(parent), ui_(new Ui::VisualizationOptions) {
	ui_->setupUi(this);
}

VisualizationOptionsWidget::~VisualizationOptionsWidget() { delete ui_; }
