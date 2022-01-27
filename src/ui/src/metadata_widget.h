#pragma once

#include <QDockWidget>
#include <QTableWidget>

namespace Ui {
class MetadataWidget;
}

class MetadataWidget : public QTableWidget {
	Q_OBJECT

 public:
	explicit MetadataWidget(QWidget* parent = nullptr);
	~MetadataWidget();
	void Test();

 private:
	QStringList lables_ = {"Parameter", "Value"};
	Ui::MetadataWidget* ui_;
};
