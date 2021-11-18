#pragma once

#include <QDockWidget>

namespace Ui {
class MetadataWidget;
}

class MetadataWidget : public QDockWidget {
	Q_OBJECT

 public:
	explicit MetadataWidget(QWidget* parent = nullptr);
	~MetadataWidget();

 private:
	Ui::MetadataWidget* ui;
};
