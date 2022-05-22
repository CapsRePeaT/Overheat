#include "visualization_options.h"

#include <QColorDialog>
#include <QDoubleSpinBox>
#include <QSlider>

#include "ui_visualization_options.h"

VisualizationOptionsWidget::VisualizationOptionsWidget(QWidget* parent)
		: QDockWidget(parent), ui_(new Ui::VisualizationOptionsWidget) {
	ui_->setupUi(this);
	auto init_color_btn = [this](QAbstractButton* btn, const QColor default_color,
	                             QColor VisualizationOptions::*color_field) {
		connect(btn, &QAbstractButton::clicked, this,
		        [this, &default_color, btn, color_field]() {
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
	init_color_btn(ui_->cold_btn, QColor("blue"),
	               &VisualizationOptions::min_temp_color);
	init_color_btn(ui_->hot_btn, QColor("red"),
	               &VisualizationOptions::max_temp_color);

	// TODO: Add default temperature updating with reader.
	auto init_temp = [this](QDoubleSpinBox* spin_box,
	                        const float default_temperature,
	                        float VisualizationOptions::*temperature_field) {
		spin_box->setValue(default_temperature);
		visualization_options_.*temperature_field = default_temperature;
		connect(spin_box, &QDoubleSpinBox::valueChanged, this,
		        [this, temperature_field](double temperature) {
							visualization_options_.*temperature_field =
									static_cast<float>(temperature);
							emit VisualizationOptionsChanged(visualization_options_);
						});
	};
	init_temp(ui_->min_temp, 20.0f, &VisualizationOptions::min_temp);
	init_temp(ui_->max_temp, 100.0f, &VisualizationOptions::max_temp);
	// temp_slider
	connect(ui_->temp_slider, &QSlider::valueChanged, this,
	        [this](int temperature) {
						visualization_options_.min_temp = temperature;
						visualization_options_.max_temp = temperature;
						emit VisualizationOptionsChanged(visualization_options_);
					});
}

VisualizationOptionsWidget::~VisualizationOptionsWidget() = default;

void VisualizationOptionsWidget::SetMinMaxTemp(float min, float max) {
	ui_->min_temp->setValue(min);
	ui_->max_temp->setValue(max);
	//ui_->temp_slider->setValue(max);
	ui_->temp_slider->setMinimum(min);
	ui_->temp_slider->setMaximum(max);
	//  visualization_options_.min_temp = min;
//  visualization_options_.max_temp = max;
//  emit VisualizationOptionsChanged(visualization_options_);
}
