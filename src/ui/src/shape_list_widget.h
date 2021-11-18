#pragma once

#include <QDockWidget>

namespace Ui {
class ShapeListWidget;
}

class ShapeListWidget : public QDockWidget {
	Q_OBJECT

 public:
	explicit ShapeListWidget(QWidget* parent = nullptr);
	~ShapeListWidget();

 private:
	Ui::ShapeListWidget* ui;
};
