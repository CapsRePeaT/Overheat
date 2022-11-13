#pragma once

#include <QDockWidget>

#include "common.h"

namespace Ui {
class VisualizationOptionsWidget;
}

struct VisualizationOptions {
	DrawMode draw_mode{};
	QColor min_temp_color{};
	QColor max_temp_color{};
	float min_temp{};
	float max_temp{};
	float stratified_step_{};
	bool show_isoterm = false;
	float isoterm_step{};
	ShowTexture texture = ShowTexture::Both;
};

// options should be saved between sessions
// should we have one object with list of options? It will lower number of
// signals and slots
class VisualizationOptionsWidget : public QDockWidget {
	Q_OBJECT

 public:
	explicit VisualizationOptionsWidget(QWidget* parent = nullptr);
	~VisualizationOptionsWidget() override;
	void SetMinMaxTemp(float min, float max);
 signals:
	void VisualizationOptionsChanged(
			const VisualizationOptions& visualization_options);
 private slots:
	void MinTempChanged(double temperature);
	void MaxTempChanged(double temperature);
	void Reset();
 private:
	void EnableStrictDrawMode(bool enabled);
	std::pair<float, float> last_setted_range_;
	std::unique_ptr<Ui::VisualizationOptionsWidget> ui_;
	VisualizationOptions visualization_options_{};
};
