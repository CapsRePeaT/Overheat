#pragma once

#include <QDockWidget>

#include "common.h"

namespace Ui {
class VisualizationOptions;
}

struct VisualizationOptions {
	DrawMode draw_mode;
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
	Ui::VisualizationOptions* ui_;
	VisualizationOptions visualization_options_;
};
