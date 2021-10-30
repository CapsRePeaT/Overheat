#ifndef VISUALISATION_OPTIONS_H
#define VISUALISATION_OPTIONS_H

#include <QWidget>

#include "common.h"

namespace Ui {
class VisualisationOptions;
}

struct VisualisationOptions {
	DrawMode draw_mode;
};

// options should be saved between sessions
// should we have one object with list of options? It will lower number of
// signals and slots
class VisualisationOptionsWidget : public QWidget {
	Q_OBJECT

 public:
	explicit VisualisationOptionsWidget(QWidget* parent = nullptr);
	~VisualisationOptionsWidget();

 signals:
	void VisualisationOptionsChanged(
			const VisualisationOptions& visualisation_options);

 private:
	Ui::VisualisationOptions* ui_;
	VisualisationOptions visualisation_options_;
};

#endif  // VISUALISATION_OPTIONS_H
