#ifndef METADATA_WIDGET_H
#define METADATA_WIDGET_H

#include <QDockWidget>

namespace Ui {
class MetadataWidget;
}

class MetadataWidget : public QDockWidget
{
  Q_OBJECT

public:
  explicit MetadataWidget(QWidget *parent = nullptr);
  ~MetadataWidget();

private:
  Ui::MetadataWidget *ui;
};

#endif // METADATA_WIDGET_H
