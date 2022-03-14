#pragma once

#include <QDockWidget>
#include <QTableWidget>

#include "common.h"


class MetadataWidget : public QTableWidget {
	Q_OBJECT

 public:
	explicit MetadataWidget(QWidget* parent = nullptr);
	~MetadataWidget() = default;
	void Test();
 public slots:
	void OnShowMetadata(const MetadataPack& pack);
 private:
	QStringList lables_ = {"Parameter", "Value"};
};
