#pragma once

#include <QDockWidget>

#include "common.h"

namespace Ui {
class VisualizationOptionsWidget;
}

struct VisualizationOptions {
	DrawMode draw_mode;
	QColor min_temp_color;
	QColor max_temp_color;
	float min_temp;
	float max_temp;
};

// options should be saved between sessions
// should we have one object with list of options? It will lower number of
// signals and slots
class VisualizationOptionsWidget : public QDockWidget {
	Q_OBJECT

 public:
	explicit VisualizationOptionsWidget(QWidget* parent = nullptr);
	~VisualizationOptionsWidget();
 signals:
	void VisualizationOptionsChanged(
			const VisualizationOptions& visualization_options);

 private:
	Ui::VisualizationOptionsWidget* ui_;
	VisualizationOptions visualization_options_;
};
