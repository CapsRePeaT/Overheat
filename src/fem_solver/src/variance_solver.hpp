#pragma once

#include "solver_shapes.hpp"

class VarianceTetraeder : public SolverTetraeder {
 public:
	//FIXME, should be 4x4
	using CoficientMatrix = Matrix;
	VarianceTetraeder(double thermal_conductivity,
		double ambient_temperature,
		double heat_flow,
		double intensity_of_heat_source,
		double convective_heat,
		std::array<VerticeIndexes::VerticeIndex, 4> indexes,
		const VerticeIndexes& index_to_coord_map
	);
	~VarianceTetraeder() = default;
	virtual void AddElementContribution(MatrixEquation& matrix) const override;
 private:
	// ai, bi, ci, di
	CoficientMatrix coef_matrix_;
	const VerticeIndexes& index_to_coord_map_;
};
