#include "renderer_widget.h"
#include "ui_renderer_widget.h"

RendererWidget::RendererWidget(QWidget *parent) :
    QWidget(parent),
    ui_(new Ui::RendererWidget)
{
    ui_->setupUi(this);
}

RendererWidget::~RendererWidget()
{
    delete ui_;
}
