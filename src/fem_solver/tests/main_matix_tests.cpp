#include <gtest/gtest.h>

#include "solver_db.hpp"



TEST(Solver2d, read_geometry) {
	MatrixEquation equation;
	equation.AddResult(0, 2);
	equation.AddResult(1, -2);
	equation.AddResult(2, 2);

	try {
		// https://www.webmath.ru/poleznoe/formules_5_7.php
		// result should be x1 = -1, x2 = 1, x3 = 3
		Matrix y(3, 1);
		y(0, 0) = 2;
		y(1, 0) = -2;
		y(2, 0) = 2;
		Matrix A(3, 3);
		//for (unsigned i = 0; i < A.size1(); ++i)
		//	for (unsigned j = 0; j < A.size2(); ++j)
		//		A(i, j) = 0.1 + 3 * i + j;
		A(0, 0) = 2; A(0, 1) = 1;  A(0, 2) = 1;
		A(1, 0) = 1; A(1, 1) = -1; A(1, 2) = 0;
		A(2, 0) = 3; A(2, 1) = -1; A(2, 2) = 2;
		//std::cout << A << std::endl;
		//identity_matrix<float> Ainv = identity_matrix<float>(A.size1());
		//permutation_matrix<size_t> pm(A.size1());
		//lu_factorize(A, pm);
		//lu_substitute(A, pm, Ainv);
		PrintMatrix(y, "input result");
		PrintMatrix(A, "input coeficients");
		Matrix Afactorized = A;
		Matrix Ainv = boost::numeric::ublas::identity_matrix<float>(A.size1());
		boost::numeric::ublas::vector<double> x_boost(Afactorized.size1(), 1);
		boost::numeric::ublas::permutation_matrix<size_t> pm(Afactorized.size1());
		Matrix result = boost::numeric::ublas::identity_matrix<float>(A.size1());
		// вырожденная
		int singular = boost::numeric::ublas::lu_factorize(Afactorized, pm);
		if (singular) {
			throw std::runtime_error("[LinearSolver<LU>::solve()] A is singular.");
		}
		result = y;
		//PrintMatrix(result, "result before");
		boost::numeric::ublas::lu_substitute(Afactorized, pm, result);
		//PrintMatrix(Afactorized, "input 2");
		//PrintMatrix(pm);
		PrintMatrix(result, "solution");
	}
}
