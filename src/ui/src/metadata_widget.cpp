#include "metadata_widget.h"

MetadataWidget::MetadataWidget(QWidget* parent)
		: QTableWidget(parent) {
	
	setColumnCount(2);
	setHorizontalHeaderLabels(lables_);
	// Test();
}

void MetadataWidget::OnShowMetadata(const MetadataPack& pack) {
	setRowCount(static_cast<int>(pack.size()));
	int row_counter = 0;
	for (const auto& metadata : pack) {
		auto* meaning = new QTableWidgetItem(
			QString::fromStdString(metadata.meaning()));
		auto* value =
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
			auto* item = new QTableWidgetItem(tr("%1").arg(pow(i, j + 1)));
			setItem(i, j, item);
		}
	}
}

