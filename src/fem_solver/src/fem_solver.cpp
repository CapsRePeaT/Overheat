#include "fem_solver.hpp"
#include "variance_solver.hpp"
#include "heatmap_converter.hpp"
#include "matrix_agregator.hpp"
#include "main_matrix_solver.hpp"

void FemSolver::Solve(FileRepresentation& file_rep) {
	std::cout << "starting heat solving..." << std::endl;
	GeometryCutter cutter;
	auto geom_db = cutter.PrepareGeometry(file_rep);
	const auto index_2_coord_map = cutter.GetVerticeIndexes();
	SolverShape* element = nullptr;
	MatrixEquation main_matrix;
	while (geom_db.NextElement(element)) {
		assert(element);
		element->AddElementContribution(main_matrix);
		delete element;
		element = nullptr;
	}
	MainMatrixSolver main_solver;
	const auto heatmap = main_solver.ComputeHeatmap(main_matrix);
	HeatmapConverter converter;
	converter.ConvertHeatmap(file_rep, heatmap);
};
