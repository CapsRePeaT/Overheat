#include "visualization_options.h"
#include "ui_visualization_options.h"

#include <QColorDialog>

VisualizationOptionsWidget::VisualizationOptionsWidget(QWidget* parent)
		: QDockWidget(parent), ui_(new Ui::VisualizationOptionsWidget) {
	ui_->setupUi(this);
	connect(ui_->cold_btn, &QAbstractButton::clicked, this,
	        [=]() { QColor color = QColorDialog::getColor(Qt::blue, this); 
	                assert(color.isValid() && "Color is not valid");
					        QString qss = QString("background-color: %1").arg(color.name());
									ui_->cold_btn->setStyleSheet(qss);
									visualization_options_.min_temp_color = color; 
									emit VisualizationOptionsChanged(visualization_options_);
		             });
}

VisualizationOptionsWidget::~VisualizationOptionsWidget() { delete ui_; }
