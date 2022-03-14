#include "metadata_widget.h"

MetadataWidget::MetadataWidget(QWidget* parent)
		: QTableWidget(parent) {
	
	setColumnCount(2);
	setHorizontalHeaderLabels(lables_);
	// Test();
}

void MetadataWidget::OnShowMetadata(const MetadataPack& pack) {
	setRowCount(pack.size());
	size_t row_counter = 0;
	for (const auto metadata : pack) {
		QTableWidgetItem* meaning = new QTableWidgetItem(
			QString::fromStdString(metadata.meaning()));
		QTableWidgetItem* value =
				new QTableWidgetItem(QString::fromStdString(metadata.value()));
		setItem(row_counter, 0, meaning);
		setItem(row_counter, 1, value);
		++row_counter;
	}
}

void MetadataWidget::Test() {
	setRowCount(20);
	// QTableWidgetItem puts data into each cell. Just an example
	for (int i = 0; i < 20; ++i) {
		for (int j = 0; j < 2; ++j) {
			QTableWidgetItem* item = new QTableWidgetItem(tr("%1").arg(pow(i, j + 1)));
			setItem(i, j, item);
		}
	}
}

