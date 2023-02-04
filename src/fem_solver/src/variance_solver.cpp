#include "variance_solver.hpp"

void VarianceTetraeder::AddElementContribution(MatrixEquation& matrix) const {
	std::cout << "Add element contribution to matrix" << std::endl;
}

VarianceTetraeder::VarianceTetraeder(double thermal_conductivity,
	double ambient_temperature,
	double heat_flow,
	double intensity_of_heat_source,
	double convective_heat,
	std::array<VerticeIndexes::VerticeIndex, 4> indexes,
	const VerticeIndexes& index_to_coord_map
) :
	SolverTetraeder(thermal_conductivity, ambient_temperature, heat_flow,
		intensity_of_heat_source, convective_heat, indexes)
	, index_to_coord_map_(index_to_coord_map), coef_matrix_(4, 4) {
	CustomPrintMatrix(coef_matrix_, "coef_matrix_");
}
