#include "solver_options.h"

#include <QLabel>



SolverOptionsWidget::SolverOptionsWidget(QWidget* parent)
		: QWidget(parent)
	  , persision_(new QDoubleSpinBox(this))
	  , show_net_(new QCheckBox("Show triangels", this))
    , test_geom_chk_(new QCheckBox("Test Geom", this)) {
	persision_->setValue(0.5);
	persision_->setSingleStep(0.25);
	persision_->setMinimum(0.1);
	show_net_->setChecked(true);
	auto layout = new QVBoxLayout(this);
	layout->addWidget(new QLabel("persision"));
	layout->addWidget(persision_);
	layout->addWidget(show_net_);
	layout->addWidget(test_geom_chk_);
	//QHBoxLayout* layout = new QHBoxLayout(this);
	setLayout(layout);
}
