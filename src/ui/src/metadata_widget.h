#pragma once

#include <QDockWidget>
#include <QTableWidget>


class MetadataWidget : public QTableWidget {
	Q_OBJECT

 public:
	explicit MetadataWidget(QWidget* parent = nullptr);
	~MetadataWidget() = default;
	void Test();

 private:
	QStringList lables_ = {"Parameter", "Value"};
};
