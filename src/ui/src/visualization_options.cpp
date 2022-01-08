#include "visualization_options.h"
#include "ui_visualization_options.h"

#include <QColorDialog>

VisualizationOptionsWidget::VisualizationOptionsWidget(QWidget* parent)
		: QDockWidget(parent), ui_(new Ui::VisualizationOptionsWidget) {
	ui_->setupUi(this);
	auto init_color_btn = [&](QAbstractButton* btn, const QColor default_color, QColor VisualizationOptions::*color_field) {
		connect(btn, &QAbstractButton::clicked, this, [=]() {
			QColor color = QColorDialog::getColor(default_color, this);
			if (color.isValid()) {  // Drop if close window without selection
				QString qss = QString("background-color: %1").arg(color.name());
				btn->setStyleSheet(qss);
				// FIXME: Add max_temp_color
				visualization_options_.*color_field = color;
				emit VisualizationOptionsChanged(visualization_options_);
			}
		});
		QString qss = QString("background-color: %1").arg(default_color.name());
		btn->setStyleSheet(qss);
	};
	init_color_btn(ui_->cold_btn, QColor("blue"), &VisualizationOptions::min_temp_color);
	init_color_btn(ui_->hot_btn, QColor("red"), &VisualizationOptions::max_temp_color);
}

VisualizationOptionsWidget::~VisualizationOptionsWidget() { delete ui_; }
