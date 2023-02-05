#pragma once

#include "solver_shapes.hpp"

class VarianceTetraeder : public SolverTetraeder {
 public:
	//FIXME, should be 4x4
	using CoficientMatrix = SparceMatrix;
	VarianceTetraeder(double thermal_conductivity,
		double ambient_temperature,
		double heat_flow,
		double intensity_of_heat_source,
		double convective_heat_coef,
		Indexes inp_indexes,
		std::array<bool, 4> convective_presense_per_side,
		std::array<bool, 4> heat_flow_presense_per_side,
		const VerticeIndexes& index_to_coord_map
	);
	~VarianceTetraeder() = default;
	virtual void AddElementContribution(MatrixEquation& matrix) const override;
 private:
	// ai, bi, ci, di
	Matrix coef_matrix_;
	const VerticeIndexes& index_to_coord_map_;
};
