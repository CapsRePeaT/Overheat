#include "visualization_options.h"
#include "ui_visualization_options.h"

#include <QColorDialog>

VisualizationOptionsWidget::VisualizationOptionsWidget(QWidget* parent)
		: QDockWidget(parent), ui_(new Ui::VisualizationOptionsWidget) {
	ui_->setupUi(this);
	auto init_color_btn = [&](QAbstractButton* btn, const QColor default_color) {
		connect(btn, &QAbstractButton::clicked, this, [=]() {
			QColor color = QColorDialog::getColor(default_color, this);
			if (color.isValid()) {  // Drop if close window without selection
				QString qss = QString("background-color: %1").arg(color.name());
				ui_->cold_btn->setStyleSheet(qss);
				visualization_options_.min_temp_color = color;
				emit VisualizationOptionsChanged(visualization_options_);
			}
		});
		QString qss = QString("background-color: %1").arg(default_color.name());
		btn->setStyleSheet(qss);
	};
	init_color_btn(ui_->cold_btn, QColor("blue"));
	init_color_btn(ui_->hot_btn, QColor("red"));
}

VisualizationOptionsWidget::~VisualizationOptionsWidget() { delete ui_; }
