#include "fem_solver.hpp"
#include "variance_solver.hpp"
#include "heatmap_converter.hpp"
#include "matrix_agregator.hpp"
#include "main_matrix_solver.hpp"

void FemSolver::Solve(FileRepresentation& file_rep) {
	std::cout << "starting heat solving..." << std::endl;
	GeometryCutter cutter;
	const auto geom_db = cutter.PrepareGeometry(file_rep);
	VarianceSolver v_solver;	
	const auto fs_matrixes = v_solver.ComputeMatrix(geom_db);
	MatrixAgregator agregator;
	const auto main_matrix = agregator.AgregateFsResultMatrix(fs_matrixes);
	MainMatrixSolver main_solver;
	const auto heatmap = main_solver.ComputeHeatmap(main_matrix);
	HeatmapConverter converter;
	converter.ConvertHeatmap(file_rep, heatmap);
};
