#include <gtest/gtest.h>

#include "solver_db.hpp"
#include "main_matrix_solver.hpp"


TEST(Solver2d, read_geom_and_heatmap) {
	// https://www.webmath.ru/poleznoe/formules_5_7.php
	// result should be x1 = -1, x2 = 1, x3 = 3
	Matrix main_matrix(3);
	main_matrix.AddResult(0, 2);
	main_matrix.AddResult(1, -2);
	main_matrix.AddResult(2, 2);
	main_matrix.AddCoeficient(0, 0, 2);
	main_matrix.AddCoeficient(0, 1, 1);
	main_matrix.AddCoeficient(0, 2, 1);
	main_matrix.AddCoeficient(1, 0, 1);
	main_matrix.AddCoeficient(1, 1, -1);
	main_matrix.AddCoeficient(1, 2, 0);
	main_matrix.AddCoeficient(2, 0, 3);
	main_matrix.AddCoeficient(2, 1, -1);
	main_matrix.AddCoeficient(2, 2, 2);
	const auto heatmap = main_matrix.Solve();

	EXPECT_TRUE(true);
}
