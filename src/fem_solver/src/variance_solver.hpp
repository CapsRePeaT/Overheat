#pragma once

#include "solver_shapes.hpp"

class VarianceTetraeder : public SolverTetraeder {
 public:
	//FIXME, should be 4x4
	using CoficientMatrix = double;
	VarianceTetraeder(double thermal_conductivity,
		double ambient_temperature,
		double heat_flow,
		double intensity_of_heat_source,
		double convective_heat,
		std::array<VerticeIndexes::VerticeIndex, 4> indexes,
		const VerticeIndexes& index_to_coord_map
		) :
		SolverTetraeder(thermal_conductivity, ambient_temperature, heat_flow, 
			            intensity_of_heat_source, convective_heat, indexes)
		, index_to_coord_map_(index_to_coord_map) {}
	~VarianceTetraeder() = default;
	virtual void AddElementContribution(MainMatrix& matrix) const override;
 private:
	CoficientMatrix coef_matrix_ = 0;
	const VerticeIndexes& index_to_coord_map_;
};
