#include "visualization_options.h"

#include <QColorDialog>
#include <QDoubleSpinBox>
#include <QRadioButton>
#include <QGroupBox>
#include <QSlider>
#include <QCheckBox>

#include "ui_visualization_options.h"

VisualizationOptionsWidget::VisualizationOptionsWidget(QWidget* parent)
		: QDockWidget(parent), ui_(new Ui::VisualizationOptionsWidget) {
	ui_->setupUi(this);
	// isotherm
	connect(ui_->isoterm, &QCheckBox::stateChanged, 
		      this, [this](int state) {
						assert(state != Qt::CheckState::PartiallyChecked);
						bool is_checked = static_cast<bool>(state);
						visualization_options_.show_isoterm = is_checked;
						ui_->isoterm_step->setEnabled(is_checked);
						emit VisualizationOptionsChanged(visualization_options_);
	});
	connect(ui_->isoterm_step, &QDoubleSpinBox::valueChanged, this,
	        [this](double step) {
						visualization_options_.isoterm_step = static_cast<float>(step);
						emit VisualizationOptionsChanged(visualization_options_);
					});
	ui_->isoterm_step->setValue(1);
	ui_->isoterm->setCheckState(Qt::CheckState::Checked);
	ui_->isoterm->setCheckState(Qt::CheckState::Unchecked);
	// draw mode
	connect(ui_->gradient, &QRadioButton::clicked, this, [this]() {
						visualization_options_.draw_mode = DrawMode::Gradient;
						EnableStrictDrawMode(false);
					});
	connect(ui_->stratified, &QRadioButton::clicked, this, [this]() {
						visualization_options_.draw_mode = DrawMode::Stratified;
		        EnableStrictDrawMode(false);  
	        });
	connect(ui_->strict, &QRadioButton::clicked, this, [this]() {
						visualization_options_.draw_mode = DrawMode::Gradient;
						EnableStrictDrawMode(true);
	        });
	visualization_options_.stratified_step_ =
			static_cast<float>(ui_->stratified_step->value());
	connect(ui_->stratified_step, &QDoubleSpinBox::valueChanged, this,
	        [this](double step) {
						visualization_options_.stratified_step_ = static_cast<float>(step);
						emit VisualizationOptionsChanged(visualization_options_);
					});
	ui_->stratified_step->setValue(1);
	// coloring
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

	// min/max temp range
	connect(ui_->min_temp, &QDoubleSpinBox::valueChanged, this,
	        &VisualizationOptionsWidget::MinTempChanged);
	connect(ui_->max_temp, &QDoubleSpinBox::valueChanged, this,
	        &VisualizationOptionsWidget::MaxTempChanged);
	// strict mode
	connect(ui_->strict_temp_slider, &QSlider::valueChanged, this,
	        [this](int temperature) {
						if (ui_->strict_temp_slider->isEnabled()) {
							visualization_options_.min_temp = temperature;
							visualization_options_.max_temp = temperature;
							emit VisualizationOptionsChanged(visualization_options_);
						}
					});
	connect(ui_->strict_temp_slider, &QSlider::valueChanged,
	        ui_->strict_temp_spin, &QDoubleSpinBox::setValue);
	connect(ui_->strict_temp_spin, &QDoubleSpinBox::valueChanged,
	        ui_->strict_temp_slider, &QSlider::setValue);
	SetMinMaxTemp(20, 100);
	ui_->gradient->click();
	connect(ui_->strict_above, &QRadioButton::clicked, this, [this]() {
		visualization_options_.texture = ShowTexture::Above;
		emit VisualizationOptionsChanged(visualization_options_);
	});
	connect(ui_->strict_below, &QRadioButton::clicked, this, [this]() {
		visualization_options_.texture = ShowTexture::Below;
		emit VisualizationOptionsChanged(visualization_options_);
	});
	connect(ui_->strict_both, &QRadioButton::clicked, this, [this]() {
		visualization_options_.texture = ShowTexture::Both;
		emit VisualizationOptionsChanged(visualization_options_);
	});
	// reset
	connect(ui_->reset, &QAbstractButton::clicked, 
	      	this,  &VisualizationOptionsWidget::Reset);
}

VisualizationOptionsWidget::~VisualizationOptionsWidget() = default;

void VisualizationOptionsWidget::SetMinMaxTemp(float min, float max) {
	//  visualization_options_.min_temp = min;
//  visualization_options_.max_temp = max;
//  emit VisualizationOptionsChanged(visualization_options_);
	last_setted_range_ = std::make_pair(min, max);
	MinTempChanged(min);
	MaxTempChanged(max);
}

void VisualizationOptionsWidget::MinTempChanged(double temperature) {
	// if to avoid loop
	if (ui_->min_temp->value() != temperature)
		ui_->min_temp->setValue(temperature);
	ui_->strict_temp_slider->setMinimum(temperature);
	visualization_options_.min_temp = static_cast<float>(temperature);
	emit VisualizationOptionsChanged(visualization_options_);
}

void VisualizationOptionsWidget::MaxTempChanged(double temperature) {
	// if to avoid loop
	if (ui_->max_temp->value() != temperature)
		ui_->max_temp->setValue(temperature);
	ui_->strict_temp_slider->setMaximum(temperature);
	visualization_options_.max_temp = static_cast<float>(temperature);
	emit VisualizationOptionsChanged(visualization_options_);
}

void VisualizationOptionsWidget::Reset() {
	SetMinMaxTemp(last_setted_range_.first, last_setted_range_.second);
}

void VisualizationOptionsWidget::EnableStrictDrawMode(bool enabled) {
	ui_->strict_temp_slider->setEnabled(enabled);
	ui_->strict_above->setEnabled(enabled);
	ui_->strict_below->setEnabled(enabled);
	ui_->strict_both->setEnabled(enabled);
	ui_->label_7->setEnabled(enabled);
	ui_->strict_temp_spin->setEnabled(enabled);
	if (enabled) {
		visualization_options_.min_temp 
			= visualization_options_.max_temp 
			= ui_->strict_temp_slider->value();
	} else {
		visualization_options_.min_temp = ui_->min_temp->value();
		visualization_options_.max_temp = ui_->max_temp->value();
	}
	emit VisualizationOptionsChanged(visualization_options_);
}
