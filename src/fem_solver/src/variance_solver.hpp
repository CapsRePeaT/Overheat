#pragma once

#include "solver_shapes.hpp"

class VarianceTetraeder : public SolverTetraeder {
 public:
	// FIXME, should be 4x4
	using CoficientMatrix = double;
	VarianceTetraeder(
			double thermal_conductivity,
			double ambient_temperature,  // env_thermal_conductivity_
			double heat_flow,            // уточнить
			double intensity_of_heat_source,  // мощность пропорциональна обьему
	                                      // power* total_colume/n_tethra
			double convective_heat, // коэффициент конвективного теплообмена на верхней поверхности корпуса
			std::array<VerticeIndexes::VerticeIndex, 4> indexes,  //
			const VerticeIndexes& index_to_coord_map)
			: SolverTetraeder(thermal_conductivity, ambient_temperature, heat_flow,
	                      intensity_of_heat_source, convective_heat, indexes),
				index_to_coord_map_(index_to_coord_map) {}
	~VarianceTetraeder() = default;
	virtual void AddElementContribution(MatrixEquation& matrix) const override;

 private:
	CoficientMatrix coef_matrix_ = 0;
	const VerticeIndexes& index_to_coord_map_;
};
