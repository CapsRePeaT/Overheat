#include "fem_solver.hpp"
#include "variance_solver.hpp"
#include "heatmap_converter.hpp"
#include "matrix_agregator.hpp"
#include "main_matrix_solver.hpp"

void FemSolver::Solve(FileRepresentation& file_rep) {
	std::cout << "starting heat solving..." << std::endl;

	auto corner_points_step = 500;
	auto area_constraint    = [](const double step) -> double {
    return std::pow(step, 2) * 2.5;
	};
	// regular tetrahedron volume
	auto volume_constraint = [](const double step) -> double {
		return 1 / 12 * std::pow(step, 3) * std::sqrt(2.0);
	};

	GeometryCutter cutter(corner_points_step, area_constraint, volume_constraint);

	auto geom_db = cutter.PrepareGeometry(file_rep, true);
	const auto index_2_coord_map = cutter.GetVerticeIndexes();
	SolverShape* element = nullptr;
	MatrixEquation main_matrix(index_2_coord_map.MaxIndex() + 1);
	while (geom_db.NextElement(element)) {
		assert(element);
		element->AddElementContribution(main_matrix);
		delete element;
		element = nullptr;
	}
	// TODO move to tests
	//main_matrix.AddResult(0, 2);
	//main_matrix.AddResult(1, -2);
	//main_matrix.AddResult(2, 2);
	//main_matrix.AddCoeficient(0, 0, 2); main_matrix.AddCoeficient(0, 1, 1); main_matrix.AddCoeficient(0, 2, 1);
	//main_matrix.AddCoeficient(1, 0, 1); main_matrix.AddCoeficient(1, 1, -1); main_matrix.AddCoeficient(1, 2, 0);
	//main_matrix.AddCoeficient(2, 0, 3); main_matrix.AddCoeficient(2, 1, -1); main_matrix.AddCoeficient(2, 2, 2);
	const auto heatmap = main_matrix.Solve();
	// https://www.webmath.ru/poleznoe/formules_5_7.php
	// result should be x1 = -1, x2 = 1, x3 = 3
	heatmap.Print();
	HeatmapConverter converter;
	converter.ConvertHeatmap(file_rep, heatmap);
};
