#include "metadata_widget.h"

#include "ui_metadata_widget.h"

MetadataWidget::MetadataWidget(QWidget* parent)
		: QDockWidget(parent), ui_(new Ui::MetadataWidget) {
	ui_->setupUi(this);
}

MetadataWidget::~MetadataWidget() { delete ui_; }
