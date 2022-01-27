#include "metadata_widget.h"

#include "ui_metadata_widget.h"

MetadataWidget::MetadataWidget(QWidget* parent)
		: QTableWidget(parent), ui_(new Ui::MetadataWidget) {
	ui_->setupUi(this);
	
	setColumnCount(2);
	setRowCount(20);
	setHorizontalHeaderLabels(lables_);
	Test();
}

void MetadataWidget::Test() {
	

	// QTableWidgetItem puts data into each cell. Just an example
	for (int i = 0; i < 20; i++) {
		for (int j = 0; j < 2; j++) {
			QTableWidgetItem* item = new QTableWidgetItem(tr("%1").arg(pow(i, j + 1)));
			setItem(i, j, item);
		}
	}
}

MetadataWidget::~MetadataWidget() { delete ui_; }
