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
	static const std::array<Matrix, 4>& matrixes_by_side();
	void ComputeThermalConductivityMatrix(const Matrix& co_factor,
		const std::array<bool, 4>& convective_presense_per_side,
		const std::array<double, 4>& side_square);
	void ComputeFlux(const std::array<bool, 4>& convective_presense_per_side,
		             const std::array<bool, 4>& heat_flow_presense_per_side,
		             const std::array<double, 4>& side_square);
	Matrix ComputeCoFactor(const Matrix& coordinates_and_coef);
	std::array<double, 4> ComputeSideSquare();
	// equal to 6 volumes of tethraeder, treat it as volume, so negative volume makes no sence
	double coord_det;
	Matrix thermal_conductivity_matrix_;
	Matrix flux_;
	const VerticeIndexes& index_to_coord_map_;
};
