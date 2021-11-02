#include "metadata_widget.h"
#include "ui_metadata_widget.h"

MetadataWidget::MetadataWidget(QWidget *parent) :
  QDockWidget(parent),
  ui(new Ui::MetadataWidget)
{
  ui->setupUi(this);
}

MetadataWidget::~MetadataWidget()
{
  delete ui;
}
