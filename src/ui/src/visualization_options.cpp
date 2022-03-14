#include "visualization_options.h"
#include "ui_visualization_options.h"

#include <QColorDialog>
#include <QDoubleSpinBox>

VisualizationOptionsWidget::VisualizationOptionsWidget(QWidget* parent)
		: QDockWidget(parent), ui_(new Ui::VisualizationOptionsWidget) {
	ui_->setupUi(this);
	auto init_color_btn = [&](QAbstractButton* btn, const QColor default_color, QColor VisualizationOptions::*color_field) {
		connect(btn, &QAbstractButton::clicked, this, [=]() {
			QColor color = QColorDialog::getColor(default_color, this);
			if (color.isValid()) {  // Drop if close window without selection
				QString qss = QString("background-color: %1").arg(color.name());
				btn->setStyleSheet(qss);
				visualization_options_.*color_field = color;
				emit VisualizationOptionsChanged(visualization_options_);
			}
		});
		visualization_options_.*color_field = default_color;
		QString qss = QString("background-color: %1").arg(default_color.name());
		btn->setStyleSheet(qss);
	};
	init_color_btn(ui_->cold_btn, QColor("blue"), &VisualizationOptions::min_temp_color);
	init_color_btn(ui_->hot_btn, QColor("red"), &VisualizationOptions::max_temp_color);


	//TODO: Add default temperature updating with reader.
	auto init_temp = [&](QDoubleSpinBox* spin_box, float default_temperature,
	                     float VisualizationOptions::*temperature_field) {
		spin_box->setValue(default_temperature);
		visualization_options_.*temperature_field = default_temperature;
		connect(spin_box, &QDoubleSpinBox::valueChanged, this,
		        [=](double temperature) {
		  visualization_options_.*temperature_field = static_cast<float>(temperature);
			emit VisualizationOptionsChanged(visualization_options_);
		});
	};
	init_temp(ui_->min_temp, 20.0f, &VisualizationOptions::min_temp);
	init_temp(ui_->max_temp, 100.0f, &VisualizationOptions::max_temp);
}

VisualizationOptionsWidget::~VisualizationOptionsWidget() { delete ui_; }
