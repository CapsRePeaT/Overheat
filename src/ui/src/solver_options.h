#pragma once

#include <QDockWidget>
#include <QWidget>
#include <QDoubleSpinBox>
#include <QCheckBox>

#include <QBoxLayout>
#include <QGridLayout>

#include "common.h"
#include "databases.h"


class SolverOptionsWidget : public QWidget {
	Q_OBJECT

 public:
	 SolverOptionsWidget(QWidget* parent = nullptr);
	SolverSetup GetSolverSetup() const {
		SolverSetup result;
		result.corner_points_step = persision_->value();
		result.show_triangulation = show_net_->isChecked();
		result.calculate_test_geometry = test_geom_chk_->isChecked();
		return result;
	}
 public slots:
 private:
	 QDoubleSpinBox* persision_ = nullptr;
	 QCheckBox* show_net_ = nullptr;
	 QCheckBox* test_geom_chk_ = nullptr;
};
