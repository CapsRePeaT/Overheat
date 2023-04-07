#include "solver_options.h"

#include <QLabel>




SolverOptionsWidget::SolverOptionsWidget(QWidget* parent)
		: QWidget(parent)
	  , persision_(new QDoubleSpinBox(this))
	  , show_net_(new QCheckBox("Show triangels", this))
    , test_geom_chk_(new QCheckBox("Test Geom", this))
		, matrix_solver_(new QComboBox(this)) 
	  , fem_solver_(new QComboBox(this)) {
	auto layout = new QVBoxLayout(this);
	layout->addWidget(new QLabel("persision"));
	// persision_
	persision_->setValue(0.5);
	persision_->setSingleStep(0.25);
	persision_->setMinimum(0.1);
	layout->addWidget(persision_);
	show_net_->setChecked(true);
	layout->addWidget(show_net_);
	layout->addWidget(test_geom_chk_);
	// Matrix solver
	layout->addWidget(new QLabel("Matrix solver"));
	layout->addWidget(matrix_solver_);
	QVariant ms_0;
	ms_0.setValue(MainMatrixType::hybrid);
	matrix_solver_->addItem("Hybrid", ms_0);
	QVariant ms_1;
	ms_1.setValue(MainMatrixType::ilu);
	matrix_solver_->addItem("ILU", ms_1);
	QVariant ms_2;
	ms_2.setValue(MainMatrixType::lu);
	matrix_solver_->addItem("LU", ms_2);
	QVariant ms_3;
	ms_3.setValue(MainMatrixType::test);
	matrix_solver_->addItem("Test", ms_3);
	// Fem solver
	layout->addWidget(new QLabel("Fem solver"));
	layout->addWidget(fem_solver_);
	QVariant fs_0;
	ms_2.setValue(FemSolverType::variance);
	fem_solver_->addItem("Variance");
	setLayout(layout);
}

SolverSetup SolverOptionsWidget::GetSolverSetup() const {
	SolverSetup result;
	result.corner_points_step = persision_->value();
	result.show_triangulation = show_net_->isChecked();
	result.calculate_test_geometry = test_geom_chk_->isChecked();
	result.fem_solver_type = matrix_solver_->currentData().value<FemSolverType>();
	result.matrix_solver_type = matrix_solver_->currentData().value<MainMatrixType>();
	return result;
}
