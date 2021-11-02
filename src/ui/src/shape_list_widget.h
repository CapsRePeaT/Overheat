#ifndef SHAPE_LIST_WIDGET_H
#define SHAPE_LIST_WIDGET_H

#include <QDockWidget>

namespace Ui {
class ShapeListWidget;
}

class ShapeListWidget : public QDockWidget
{
  Q_OBJECT

public:
  explicit ShapeListWidget(QWidget *parent = nullptr);
  ~ShapeListWidget();

private:
  Ui::ShapeListWidget *ui;
};

#endif // SHAPE_LIST_WIDGET_H
