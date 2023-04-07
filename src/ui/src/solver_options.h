#pragma once

#include <QDockWidget>
#include <QWidget>
#include <QDoubleSpinBox>
#include <QCheckBox>

#include <QBoxLayout>
#include <QComboBox>
#include <QGridLayout>

#include "common.h"
#include "databases.h"

Q_DECLARE_METATYPE(MainMatrixType);
Q_DECLARE_METATYPE(FemSolverType);

class SolverOptionsWidget : public QWidget {
	Q_OBJECT

 public:
	 SolverOptionsWidget(QWidget* parent = nullptr);
	 SolverSetup GetSolverSetup() const;
 public slots:
 private:
	 QDoubleSpinBox* persision_ = nullptr;
	 QCheckBox* show_net_ = nullptr;
	 QCheckBox* test_geom_chk_ = nullptr;
	 QComboBox* matrix_solver_ = nullptr;
	 QComboBox* fem_solver_ = nullptr;
};
