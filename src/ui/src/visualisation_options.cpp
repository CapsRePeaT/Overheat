#include "visualisation_options.h"

#include "ui_visualisation_options.h"

VisualisationOptionsWidget::VisualisationOptionsWidget(QWidget* parent)
		: QWidget(parent), ui_(new Ui::VisualisationOptions) {
	ui_->setupUi(this);
}

VisualisationOptionsWidget::~VisualisationOptionsWidget() { delete ui_; }
